clear all
close all

%fichier1 = input("cosinus.txt");
%fichier2 = input("sinus.txt");

Eps_cos = load("cosinus.txt")
Eps_sin = load("sinus.txt");
A = (0:90);
Phi = A';

subplot(1,2,1);
plot(Phi,Eps_cos);
legend("Ecos")
hold
subplot(1,2,2);
plot(Phi,Eps_sin);
legend("Esin");