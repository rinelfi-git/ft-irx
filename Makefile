NAME = ircserv
CPPC = c++
CPPARG = -Wall -Wextra -Werror -std=c++98
SRC_DIR = sources
OBJ_DIR = objects
SRCs = \
	abstract/ASocketClient.cpp \
	abstract/ASocketServerObserver.cpp \
	ArgValidator.cpp \
	IRCServer.cpp \
	Pending.cpp \
	SocketServer.cpp \
	UserInfo.cpp \
	utils.cpp \
	main.cpp
OBJ_PATH = $(addprefix $(OBJ_DIR)/,$(SRCs:.cpp=.o))
SRC_PATH = $(addprefix $(SRC_DIR)/,$(SRCs))
INCLUDE_PATH = -I./headers -I./headers/abstract

$(NAME): $(OBJ_PATH)
	$(CPPC) $(CPPARG) $^ -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(OBJ_DIR)
	@mkdir -p $(@D)
	$(CPPC) $(CPPARG) $(INCLUDE_PATH) -c $< -o $@

all: $(NAME)

clean:
	rm -rf $(OBJ_PATH)
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -rf $(NAME)

re: fclean all

.PHONY: all clean fclean test re