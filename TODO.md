# TODO - Améliorations du serveur IRC

## 1. Refactoring du parsing - Orientation objet

### Problème actuel
- Parsing centralisé dans `Authenticated::parse()` avec une map de pointeurs de fonctions
- Code difficile à maintenir et étendre
- Violation du principe ouvert/fermé

### Solution proposée
- Créer une hiérarchie de classes de commandes IRC
- Classe abstraite `ICommand` avec méthode virtuelle `execute()`
- Une classe par commande (`JoinCommand`, `ModeCommand`, `PrivMsgCommand`, etc.)
- Factory pattern pour instancier les bonnes commandes

### Exemple d'implémentation

```cpp
// headers/commands/ICommand.hpp
class ICommand {
public:
    virtual ~ICommand() {}
    virtual void execute(const std::string& args, User& user) = 0;
};

// headers/commands/JoinCommand.hpp
class JoinCommand : public ICommand {
public:
    virtual void execute(const std::string& args, User& user);
};

// sources/commands/JoinCommand.cpp
void JoinCommand::execute(const std::string& args, User& user) {
    std::stringstream builder(args);
    std::string name, password;
    builder >> name >> password;
    
    Channel* channel = IRCServer::getInstance().channel(name);
    if (!channel)
        IRCServer::getInstance().createChannel(name, &user);
    else if (channel->auth(&user, password))
        channel->join(&user);
}

// CommandFactory.cpp
class CommandFactory {
private:
    std::map<std::string, ICommand* (*)()> _commands;
    
    static ICommand* createJoin() { return new JoinCommand(); }
    static ICommand* createMode() { return new ModeCommand(); }
    static ICommand* createPrivMsg() { return new PrivMsgCommand(); }
    
public:
    CommandFactory() {
        _commands["join"] = &createJoin;
        _commands["mode"] = &createMode;
        _commands["privmsg"] = &createPrivMsg;
        _commands["ping"] = &createPing;
        _commands["topic"] = &createTopic;
        _commands["kick"] = &createKick;
        _commands["quit"] = &createQuit;
        _commands["invite"] = &createInvite;
    }
    
    ICommand* create(const std::string& cmd) {
        std::map<std::string, ICommand* (*)()>::iterator it = _commands.find(cmd);
        return (it != _commands.end()) ? it->second() : NULL;
    }
};
```

### Bénéfices
- Code plus modulaire et extensible
- Facilite l'ajout de nouvelles commandes
- Respect des principes SOLID
- Chaque commande est isolée dans sa propre classe

## 2. Système de middleware pour les validations

### Problème actuel
- Vérifications dispersées dans chaque méthode de parsing
- Code dupliqué pour les checks d'autorisation
- If/else imbriqués difficiles à lire

### Solution proposée
- Pattern Chain of Responsibility pour les validations
- Middleware génériques réutilisables :
  - `ChannelExistsMiddleware`
  - `UserIsOperatorMiddleware`
  - `ChannelMembershipMiddleware`
  - `ParameterValidationMiddleware`

### Exemple d'implémentation

```cpp
// headers/middleware/IMiddleware.hpp
class IMiddleware {
public:
    virtual ~IMiddleware() {}
    virtual bool process(const std::string& args, User& user, std::string& error) = 0;
    void setNext(IMiddleware* next) { _next = next; }
protected:
    IMiddleware* _next;
    bool processNext(const std::string& args, User& user, std::string& error) {
        return _next ? _next->process(args, user, error) : true;
    }
};

// headers/middleware/ChannelExistsMiddleware.hpp
class ChannelExistsMiddleware : public IMiddleware {
public:
    virtual bool process(const std::string& args, User& user, std::string& error) {
        std::stringstream ss(args);
        std::string channelName;
        ss >> channelName;
        
        Channel* channel = IRCServer::getInstance().channel(channelName);
        if (!channel) {
            error = "403 " + user.info().nick() + " " + channelName + " :No such channel";
            return false;
        }
        return processNext(args, user, error);
    }
};

// headers/middleware/UserIsOperatorMiddleware.hpp
class UserIsOperatorMiddleware : public IMiddleware {
public:
    virtual bool process(const std::string& args, User& user, std::string& error) {
        std::stringstream ss(args);
        std::string channelName;
        ss >> channelName;
        
        Channel* channel = IRCServer::getInstance().channel(channelName);
        if (!channel->isOperator(user)) {
            error = "482 " + user.info().nick() + " " + channelName + " :You're not channel operator";
            return false;
        }
        return processNext(args, user, error);
    }
};

// Utilisation dans ModeCommand.cpp
void ModeCommand::execute(const std::string& args, User& user) {
    ChannelExistsMiddleware* channelCheck = new ChannelExistsMiddleware();
    UserIsOperatorMiddleware* opCheck = new UserIsOperatorMiddleware();
    
    channelCheck->setNext(opCheck);
    
    std::string error;
    if (!channelCheck->process(args, user, error)) {
        user.socket().send(error);
        delete channelCheck;
        delete opCheck;
        return;
    }
    
    // Traitement de la commande MODE...
    delete channelCheck;
    delete opCheck;
}
```

### Bénéfices
- Séparation des responsabilités
- Code plus lisible et maintenable
- Validations réutilisables
- Facilite l'ajout de nouvelles règles
- Évite les if/else imbriqués

## 3. Améliorations spécifiques détectées

### Dans `Authenticated.cpp`
- `_parseMode()`: Logique complexe avec maps imbriquées (lignes 34-46)
- Gestion inconsistante des modes +i/-i (lignes 82-91)
- Code dupliqué pour les vérifications de canaux

### Dans `Channel.cpp`
- Méthodes `auth()`, `setPassword()`, `setTopicMode()`: Checks répétitifs d'opérateur
- `modeResume()` et `modeResume(const std::string&)`: Code dupliqué (lignes 124-186)

### Dans `IRCServer.cpp`
- `onData()`: Dynamic cast répétitifs (lignes 101-111)
- Gestion des déconnexions pourrait être simplifiée

## 4. Refactoring de la gestion des réponses

### Problème actuel
- Codes de réponse IRC hardcodés partout
- Messages d'erreur duplicqués

### Solution proposée
- Énumérations pour les codes de réponse IRC
- Templates de messages centralisés
- Builder pattern pour construire les réponses

### Exemple d'implémentation

```cpp
// headers/IRCCodes.hpp
class IRCCodes {
public:
    static const int RPL_WELCOME = 001;
    static const int RPL_CHANNELMODEIS = 324;
    static const int ERR_NOSUCHNICK = 401;
    static const int ERR_NOSUCHCHANNEL = 403;
    static const int ERR_NEEDMOREPARAMS = 461;
    static const int ERR_CHANOPRIVSNEEDED = 482;
};

// headers/ResponseBuilder.hpp
class ResponseBuilder {
private:
    std::string _response;
public:
    ResponseBuilder& code(int code);
    ResponseBuilder& target(const std::string& target);
    ResponseBuilder& channel(const std::string& channel);
    ResponseBuilder& message(const std::string& msg);
    std::string build() const;
};

// sources/ResponseBuilder.cpp
ResponseBuilder& ResponseBuilder::code(int code) {
    std::stringstream ss;
    ss << std::setfill('0') << std::setw(3) << code;
    _response = ss.str();
    return *this;
}

ResponseBuilder& ResponseBuilder::target(const std::string& target) {
    _response += " " + target;
    return *this;
}

std::string ResponseBuilder::build() const {
    return _response;
}

// Utilisation dans Response.cpp
void Response::errNoSuchChannel(const std::string& user, const std::string& channel) const {
    std::string response = ResponseBuilder()
        .code(IRCCodes::ERR_NOSUCHCHANNEL)
        .target(user)
        .channel(channel)
        .message(":No such channel")
        .build();
    _client->send(response);
}
```

## 5. Amélioration de la gestion des modes

### Problème actuel
- Parsing des modes complexe et peu flexible
- Code dupliqué entre modes avec/sans paramètres

### Solution proposée
- Registry pattern pour enregistrer les modes supportés
- Classes spécialisées par type de mode
- Validation automatique des paramètres

### Exemple d'implémentation

```cpp
// headers/modes/IModeHandler.hpp
class IModeHandler {
public:
    virtual ~IModeHandler() {}
    virtual bool needsParameter(char action) const = 0;
    virtual void apply(Channel& channel, User& user, char action, const std::string& param = "") = 0;
};

// headers/modes/ModeRegistry.hpp
class ModeRegistry {
private:
    std::map<char, IModeHandler*> _handlers;
    
public:
    void registerMode(char mode, IModeHandler* handler) {
        _handlers[mode] = handler;
    }
    
    IModeHandler* getHandler(char mode) {
        std::map<char, IModeHandler*>::iterator it = _handlers.find(mode);
        return (it != _handlers.end()) ? it->second : NULL;
    }
};

// headers/modes/TopicModeHandler.hpp
class TopicModeHandler : public IModeHandler {
public:
    virtual bool needsParameter(char action) const { return false; }
    virtual void apply(Channel& channel, User& user, char action, const std::string& param) {
        channel.setTopicMode(user, action == '+');
    }
};

// headers/modes/KeyModeHandler.hpp
class KeyModeHandler : public IModeHandler {
public:
    virtual bool needsParameter(char action) const { return true; }
    virtual void apply(Channel& channel, User& user, char action, const std::string& param) {
        if (action == '+')
            channel.setPassword(user, param);
        else
            channel.setPassword(user, "");
    }
};

// Utilisation dans ModeCommand.cpp
void ModeCommand::execute(const std::string& args, User& user) {
    ModeRegistry registry;
    registry.registerMode('t', new TopicModeHandler());
    registry.registerMode('k', new KeyModeHandler());
    registry.registerMode('i', new InviteModeHandler());
    registry.registerMode('o', new OperatorModeHandler());
    registry.registerMode('l', new LimitModeHandler());
    
    // Parsing et application des modes...
    for (std::string::const_iterator it = modes.begin(); it != modes.end(); ++it) {
        char mode = *it;
        IModeHandler* handler = registry.getHandler(mode);
        if (handler) {
            if (handler->needsParameter(action) && hasParameter) {
                handler->apply(*channel, user, action, parameter);
            } else if (!handler->needsParameter(action)) {
                handler->apply(*channel, user, action);
            }
        }
    }
}
```

## 6. Optimisations mémoire

### Points d'attention
- Vérifier les fuites mémoire dans les destructeurs
- Optimiser les copies d'objets User/Channel
- Utiliser des références constantes quand possible

### Exemple d'amélioration

```cpp
// Dans Channel.cpp - éviter les copies inutiles
// AVANT (ligne 117):
User member(*(itMember++)->second);

// APRÈS:
const User& member = *(itMember++)->second;

// Dans IRCServer.cpp - utiliser des smart pointers (si autorisé en C++98)
// Ou au minimum, meilleure gestion RAII
class IRCServer {
private:
    std::vector<ICommand*> _commandsToDelete; // Pour cleanup
    
    void registerCommand(const std::string& name, ICommand* cmd) {
        _commands[name] = cmd;
        _commandsToDelete.push_back(cmd);
    }
    
    ~IRCServer() {
        // Cleanup automatique des commandes
        for (std::vector<ICommand*>::iterator it = _commandsToDelete.begin();
             it != _commandsToDelete.end(); ++it) {
            delete *it;
        }
    }
};
```

## 7. Priorités d'implémentation

1. **Haute** - Système de middleware pour les validations
2. **Haute** - Refactoring du parsing vers des classes de commandes  
3. **Moyenne** - Centralisation des codes de réponse IRC
4. **Moyenne** - Amélioration de la gestion des modes
5. **Basse** - Optimisations mémoire et performance

## 8. Notes d'implémentation

- Toutes les solutions utilisent uniquement du C++98 standard
- Pas de smart pointers, utilisation manuelle de new/delete
- Les maps remplacent les if/else imbriqués pour une meilleure maintenabilité  
- Pattern RAII pour la gestion mémoire
- Interfaces pures virtuelles pour l'extensibilité