clear all
close all

%fichier1 = input("cosinus.txt");
%fichier2 = input("sinus.txt");

Err_cos = load("cosinus.txt")
Err_sin = load("sinus.txt");
A = (0:90);
Phi = A';

subplot(1,2,1);
plot(Phi,Err_cos);
legend("Err_cos")
hold
subplot(1,2,2);
plot(Phi,Err_sin);
legend("Err_sin");