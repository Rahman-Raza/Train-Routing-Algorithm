train.out : train.o TrainRunner.o  
	g++ -ansi -Wall -pg -g -o train.out train.o TrainRunner.o  

train.o : train.cpp TrainRunner.h train.h 
	g++ -ansi -Wall -g -c train.cpp

TrainRunner.o : TrainRunner.cpp CPUTimer.h train.h TrainRunner.h 
	g++ -ansi -Wall -g -c TrainRunner.cpp

clean : 
	rm -f train.out train.o  TrainRunner.o   
