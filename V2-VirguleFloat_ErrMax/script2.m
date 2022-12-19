clear all
close all


Err_max_cos = load("ErrMaxCos.txt");
Err_max_sin = load("ErrMaxSin.txt");

A = (0:63);
iteration = A';

#subplot(1,2,1);
plot(iteration,log10(Err_max_cos));
#plot(iteration,Err_max_cos);
legend("ErrMax")


  
#hold
#subplot(1,2,2);
#plot(iteration,log10(Err_max_sin));
#plot(iteration,Err_max_sin);
#legend("Eps_sin");