# the compiler
CC = g++

# the build target executable:
TARGET  = consumer
TARGET2 = producer

all: $(TARGET) $(TARGET2)

$(TARGET): $(TARGET).cpp
	$(CC) -o $(TARGET).o $(TARGET).cpp

$(TARGET2): $(TARGET2).cpp
	$(CC) -o $(TARGET2).o $(TARGET2).cpp
