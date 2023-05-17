NAME			= ft_containers
NAME_STL		= stl_containers

CXX				= clang++

FLAGS			= -MMD -Wall -Wextra -Werror -g -std=c++98

RM				= rm -rf

HDRS			= -I src/

SRC_DIR 		= src
SRC_FT			= $(addsuffix .cpp, main_ft)
SRC_STL			= $(addsuffix .cpp, main_stl)

OBJ_DIR			= obj
OBJ_FT			= $(addprefix $(OBJ_DIR)/, $(addsuffix .o, main_ft))
OBJ_STL			= $(addprefix $(OBJ_DIR)/, $(addsuffix .o, main_stl))

OBJ_FT_BUILD	= $(addprefix $(OBJ_DIR)/, $(SRC_FT:.cpp=.o))
OBJ_STL_BUILD	= $(addprefix $(OBJ_DIR)/, $(SRC_STL:.cpp=.o))
MMD_FILES		= $(OBJ_FT_BUILD:.o=.d) $(OBJ_STL_BUILD:.o=.d)

.PHONY:			all clean fclean re

all:			$(NAME)

$(NAME):		$(OBJ_DIR) $(OBJ_FT_BUILD) $(OBJ_STL_BUILD)
				$(CXX) $(FLAGS) $(HDRS) -o $(NAME) $(OBJ_FT_BUILD)
				$(CXX) $(FLAGS) $(HDRS) -o $(NAME_STL) $(OBJ_STL_BUILD)

$(OBJ_DIR)/%.o:	$(SRC_DIR)/%.cpp
				$(CXX) $(FLAGS) $(HDRS) -o $@ -c $<

$(OBJ_DIR):
				mkdir -p $(OBJ_DIR)

ft:				$(OBJ_DIR) $(OBJ_FT_BUILD)
				$(CXX) $(FLAGS) $(HDRS) -o $(NAME) $(OBJ_FT_BUILD)

stl:			$(OBJ_DIR) $(OBJ_STL_BUILD)
				$(CXX) $(FLAGS) $(HDRS) -o $(NAME_STL) $(OBJ_STL_BUILD)

clean:
				$(RM) $(OBJ_DIR)
				$(RM) ./src/*.gch *.txt
				@echo "\033[32;1mCleaning succeed\n\033[0m"

fclean:			clean
				$(RM) $(NAME) $(NAME_STL)
				@echo "\033[33;1mAll created files were deleted\n\033[0m"

re:				fclean all
				@echo "\033[35;1mRemake done\n\033[0m"

-include $(MMD_FILES)