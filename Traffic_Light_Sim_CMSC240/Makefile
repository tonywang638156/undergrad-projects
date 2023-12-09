simulation: FinalProject.o VehicleBase.o Animator.o
	g++ FinalProject.o VehicleBase.o Animator.o -o simulation

FinalProject.o: FinalProject.cpp Animator.h VehicleBase.h
	g++ -c FinalProject.cpp

Animator.o: Animator.cpp Animator.h
	g++ -c Animator.cpp

VehicleBase.o: VehicleBase.cpp VehicleBase.h
	g++ -c VehicleBase.cpp

clean:
	rm *.o simulation


