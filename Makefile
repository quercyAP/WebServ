SRCS			= clientConnexion/ClientConnexion.cpp \
				 config/ADirective.cpp \
				 config/ConfParser.cpp \
				 config/directives/AutoIndex.cpp \
				 config/directives/CgiPhp.cpp \
				 config/directives/CgiPy.cpp \
				 config/directives/Index.cpp \
				 config/directives/LimitMethode.cpp \
				 config/directives/Listen.cpp \
				 config/directives/MaxBodySize.cpp \
				 config/directives/Redir.cpp \
				 config/directives/Root.cpp \
				 config/directives/ServerNames.cpp \
				 config/directives/Upload.cpp \
				 config/directives/Error.cpp \
				 config/LocationBlock.cpp \
				 config/Route.cpp \
				 config/ServerBlock.cpp \
				 core/Server.cpp \
				 handler/NewClientConnexionHandler.cpp \
				 handler/incomingDataHandler/IncomingDataHandler.cpp \
				 handler/incomingDataHandler/static/StaticMethodHandler.cpp \
				 handler/incomingDataHandler/static/Post.cpp \
				 handler/incomingDataHandler/static/StaticUploadHandler.cpp \
				 handler/incomingDataHandler/static/Delete.cpp \
				 handler/incomingDataHandler/static/Get.cpp \
				 handler/incomingDataHandler/ParseHeader.cpp \
				 handler/incomingDataHandler/PythonInCgiHandler.cpp \
				 handler/incomingDataHandler/Router.cpp \
				 handler/CgiCrashHandler.cpp \
				 handler/OutgoingDataHandler.cpp \
				 handler/PythonOutCgiHandler.cpp \
				 handler/PythonCgiHandler.cpp \
				 handler/CgiEnv.cpp \
				 main.cpp \
				
SRC_DIR			=	./src/
SOURCES			=	$(addprefix $(SRC_DIR), $(SRCS))

OBJ_DIR			=	./obj/
OBJ_DIR_DEBUG	=	./obj_debug/
OBJ_FILE 		= 	$(patsubst %.cpp, %.o, $(SRCS))
OBJ_FILE 		:= 	$(patsubst %.tpp, %.o, $(OBJ_FILE))
OBJ_FILE 		:= 	$(patsubst %.ipp, %.o, $(OBJ_FILE))
OBJECTS			=	$(addprefix $(OBJ_DIR), $(OBJ_FILE))
OBJECTS_DEBUG	=	$(addprefix $(OBJ_DIR_DEBUG), $(OBJ_FILE))

NAME			=	webserv
CC				=	c++
CCFLAG			=	-Wall -Wextra -Werror -std=c++98 -Wno-unknown-pragmas

HDR_FLAGS		= -I includes \
				 -I includes/clientConnexion \
				 -I includes/config \
				 -I includes/config/directives \
				 -I includes/core \
				 -I includes/handler \
				 -I includes/handler/incomingDataHandler \
				 -I includes/handler/incomingDataHandler/static \
				 -I includes/response \
				 -I includes/utils \
				
LIB				=		
LIB_DIR			=	

all						:	$(NAME)

$(NAME)					:	obj $(OBJ_DIR) $(OBJECTS)
							@$(CC) $(CCFLAG) $(OBJECTS) $(HDR_FLAGS) $(LIB_DIR) $(LIB) -o $(NAME)
							@echo "\033[0;92m\n* $(NAME) program file was created *\033[0m"

$(OBJ_DIR)%.o 			: 	$(SRC_DIR)%.cpp
							@$(CC) $(CCFLAGS) $(HDR_FLAGS)-c $< -o $@
							@echo "\033[0;96m* $(notdir $<) -> $(notdir $@) * \033[0m: \033[0;92mOK\033[0m"

$(OBJ_DIR_DEBUG)%.o		: 	$(SRC_DIR)%.cpp
							@$(CC) -g $(CCFLAGS) $(HDR_FLAGS) -c $< -o $@
							@echo "\033[0;96m* $(notdir $<) -> $(notdir $@) * \033[0m: \033[0;93mOK (debug)\033[0m"


clean					:
							@rm -rf $(OBJ_DIR) && rm -rf $(OBJ_DIR_DEBUG)
							@echo "\033[0;91m* $(NAME) object files were deleted *\033[0m"

fclean					:	clean 
							@rm -rf $(NAME)
							@echo "\033[0;91m* $(NAME) was deleted *\033[0m"

re						:	fclean $(NAME)

sanitize				:	fclean obj_debug $(OBJ_DIR_DEBUG) $(OBJECTS_DEBUG)
							@$(CC) -fsanitize=address -g3 $(CCFLAG) $(OBJECTS_DEBUG) $(LIB_DIR) $(LIB) -o $(NAME)
							@echo "\033[0;93m\n* $(NAME) program file was created in sanitize mode*\033[0m"

debug					:	fclean obj_debug $(OBJ_DIR_DEBUG) $(OBJECTS_DEBUG)
							@$(CC) -g $(CCFLAG) $(OBJECTS_DEBUG) $(LIB_DIR) $(LIB) -o $(NAME)
							@echo "\033[0;93m\n* $(NAME) program file was created in debug mode*\033[0m"


add						:
							@script="tools/update_srcs.sh"; \
							while [ ! -f $$script ] && [ ! $$PWD = "/" ]; do cd ..; done; \
							if [ -f $$script ]; then \
								./$$script "$(CURDIR)"; \
								echo "SRCS File added successfully."; \
							else \
								echo "update_srcs.sh not found in parent directories"; \
							fi

create					:
							@echo "Enter the names of your repertory and files (separated by spaces):"
							@read args; \
							./tools/new_c_h_creator.sh $$args; \
							make add

obj						:
							@mkdir -p obj
							@dirs=$$(find "src/" -type d | sed "s|^src/||"); \
							for dir in $$dirs; do \
								mkdir -p obj/$$dir; \
								echo "\033[0;92m* obj$$dir dir created *\033[0m"; \
							done

obj_debug				:
							@mkdir -p obj_debug
							@dirs=$$(find "src/" -type d | sed "s|^src/||"); \
							for dir in $$dirs; do \
								mkdir -p obj_debug/$$dir; \
								echo "\033[0;92m* obj_debug$$dir dir created *\033[0m"; \
							done



.PHONY					:	all clean fclean re sanitize debug obj obj_debug create add
