NAME := webserv

SRC_DIR := src
CONFIG_DIR := $(SRC_DIR)/Config
HEADER_DIR := includes
OBJ_DIR := objs
OBJS_DIR := $(OBJ_DIR) $(addprefix $(OBJ_DIR)/, $(SRC_DIR))

HEADERS := $(addprefix $(HEADER_DIR)/, Config.hpp)

SOURCE := $(addprefix $(SRC_DIR)/, main.cpp) \
		  $(addprefix $(CONFIG_DIR)/, setter.cpp parsing.cpp getter.cpp test.cpp \
			 Config.cpp)

OBJS := $(SOURCE:%.cpp=$(OBJ_DIR)/%.o)

RM := rm
CC := c++
MAKE := make
MAKE_DIR := mkdir -p

RM_FLAG := -rf
CC_FLAG := -Wall -Wextra -Werror -std=c++11

all: $(NAME)

$(NAME): $(OBJ_DIR) $(OBJS) $(HEADERS)
	$(CC) $(CC_FLAG) $(OBJS) -o $(NAME)
	@echo "$(GREEN)Build successfully!$(NC)"

clean:
	@$(RM) $(RM_FLAG) $(OBJ_DIR) $(LIBFT_DIR)/objs
	@echo "$(GREEN)Clean bins successfully!$(NC)"

fclean: clean
	@$(RM) $(RM_FLAG) $(NAME)
	@echo "$(GREEN)Clean everything successfully!$(NC)"

re: fclean all

$(OBJ_DIR)/%.o: %.cpp $(HEADERS)
	@$(MAKE_DIR) $(dir $@)  # Create the directory for the object file if it doesn't exist
	$(CC) $(CC_FLAG) -c $< -o $@

$(OBJ_DIR):
	@echo "$(BLUE)Start compiling...$(NC)"
	$(MAKE_DIR) $(OBJS_DIR)

.PHONY: all clean fclean re

GREEN := \033[0;32m
BLUE := \033[0;34m
NC := \033[0m
