CC= @gcc -Wall -Werror -Wextra -std=c11
TARGET= s21_grep


all: $(TARGET)

rebuild: clean all

clean:
	@rm -rf ./$(TARGET)

$(TARGET): 
	$(CC) ./$(TARGET).c -o ./$(TARGET)