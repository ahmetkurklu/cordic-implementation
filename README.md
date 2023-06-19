# Design of Digital Systems / Hardware Implementation of CORDIC Algorithm
## I. Introduction
The CORDIC algorithm (COordinate Rotation Digital Computing) is a simple and efficient method for calculating a range of complex functions. Based on a combination of iterative shift and vector rotation operations, the algorithm approximates most trigonometric functions. Many commercial calculators use this algorithm to solve functions such as sine, cosine, arctangent, and square root. Digital communication systems also use it to quickly perform polar-to-cartesian conversions on sampled signals. Like most efficient algorithms, the effectiveness of CORDIC lies in its simplicity and flexibility. The algorithm consists of basic mathematical operations, which not only provides high computational efficiency but also simplifies its implementation on software or hardware platforms. The following project proposes a brief experience on the complete design flow of hardware implementation for a CORDIC calculator. Starting from a C model of the algorithm, the goal is to obtain a synthesizable VHDL description for an FPGA target. 
The objectives of this practical work are as follows: 
* Illustrate the complementarity between high-level behavioral models (C) and models used for hardware synthesis (VHDL). 
* Familiarize with implementation techniques for complex numerical functions in hardware (such as fixed-point arithmetic). 
* Learn to use a complete development chain for FPGA.

## II. Task description
The goal of this project is to create a circuit that calculates the sine and cosine of an angle using the algorithm presented in the previous section. The circuit and its temporal behavior are explained with the next figures.
![cordic](https://github.com/ahmetkurklu/cordic-implementation/assets/24780090/ea429ff1-d842-473b-970b-a6a00963b89e)

The value of the angle (phi) and the number of iterations (n) are provided as inputs. The availability of these operands is indicated by setting the "start" signal to 1. The circuit acknowledges receipt by setting "rdy" to 0. The operands are then internally stored, and the client can set "start" back to 0. The end of the calculation is signaled by the rising edge of "rdy" to 1. The results, cos(phi) and sin(phi), are then available on the outputs r1 and r2, respectively. The behavior is undefined if "start" transitions to 1 while "rdy" is 0. The "rst" signal allows resetting the circuit.
![chronograme](https://github.com/ahmetkurklu/cordic-implementation/assets/24780090/6ce15c91-64be-4afb-b427-e6f1b0d7a708)

## II.1. Step-by-Step Approach
The step-by-step approach for implementing the circuit for calculating the sine and cosine of an angle using the CORDIC algorithm is as follows:

1. Write and validate the algorithm in C using floating-point numbers.
2. Modify the C model to operate on fixed-point numbers and validate it.
3. Translate the fixed-point C model into VHDL using a behavioral model and validate it.
4. Transform the VHDL behavioral model into an architectural model and perform simulation.
5. Synthesize the VHDL code and estimate the performance.

## II.2. C Model in Floating-Point
By following the principles provided in Section II.1 of the project, we have written a function named cordic() in C that calculates the sine and cosine of an angle. The function is as follows (you can find the complete code in the directory [V1-VirguleFloat_ErrCosSIn/main.c](https://github.com/ahmetkurklu/cordic-implementation/blob/main/V1-VirguleFloat_ErrCosSIn/main.c)
):

The cordic() function takes the following arguments:

* phi: The angle expressed in degrees and encoded as a double.
* n: The number of iterations.
* x and y: Input/output variables used to retrieve the results.

Before implementing the cordic() function, we have created two arrays of type double to store the constants θi and Ai. These arrays are declared as global variables and filled during the execution of the initialisationTab() function at the beginning of main().

The functions RtoD() and DotR() allow for the conversion between radians and degrees, respectively.

## 2.1. Elementary Tests
To test the functionality of our program, we have implemented a function named test(). The function prompts the user to enter an angle and the number of iterations, and then displays the values of x and y, which represent cos(phi) and sin(phi), respectively.
When we run the test() function with 30 iterations on an angle of 60 degrees, we obtain the following result:

* cos(phi) = 0.5000000000000000
* sin(phi) = 0.8660254037844386

We can compare these values with the ones shown in the next figure. It is evident that the obtained values for x and y are similar to the values indicated in the table, confirming that our cordic() function is functioning correctly.

![image](https://github.com/ahmetkurklu/cordic-implementation/assets/24780090/5ed80da8-3db0-41a8-b0fa-34b64c885e26)

## II.2.2. Qualification

To perform the first step of the qualification process for plotting the curves of Esin(phi) and Ecos(phi), we wrote quali() function. This function generates two output files, cosinus and sinus. We wrote an Octave script located in the directory [V1-VirguleFloat_ErrCosSIn/script1.m](https://github.com/ahmetkurklu/cordic-implementation/blob/main/V1-VirguleFloat_ErrCosSIn/script1.m) to plot the curves. We plot the curves for a number of iterations equal to 4, 8, 19, 32, and finally 64. The resulting curves are as follows: the left side shows the error on the cosine, and the right side shows the error on the sine.

![image](https://github.com/ahmetkurklu/cordic-implementation/assets/24780090/9540e87f-55e8-4fb1-9906-2caaa422fc59)

*Error of cosine and sine for n = 4*

![image](https://github.com/ahmetkurklu/cordic-implementation/assets/24780090/d9af5020-3066-4e0c-953c-c96effa0e932)

*Error of cosine and sine for n = 8*

![image](https://github.com/ahmetkurklu/cordic-implementation/assets/24780090/a6914fcb-da19-4421-aafd-5ef4e8899ead)

*Error of cosine and sine for n = 16*

![image](https://github.com/ahmetkurklu/cordic-implementation/assets/24780090/ef6ce253-20d7-4d66-8670-9fcc99bec87d)

*Error of cosine and sine for n = 32*

![image](https://github.com/ahmetkurklu/cordic-implementation/assets/24780090/f0d79d39-2e3f-4e1a-9e7c-e4545c007929)

*Error of cosine and sine for n = 64*

We observe that for n = 4, 8, 16, and 32, the curves for cosine and sine are symmetric. This is due to the algorithm, as when alpha > phi, we add the sine and subtract the cosine, and when alpha < phi, we subtract the sine and add the cosine. Hence, we obtain the same curves but inverted. For n = 64, we notice that the symmetry is lost, which can be explained by the quantization noise. Additionally, we observe that the error for cosine and sine decreases with an increase in the number of iterations.

For n = 4, the maximum error is approximately 0.12. For n = 8, the error is 0.008. For n = 32, the error is around 10^(-10). Finally, for n = 64, we observe a maximum error of approximately 10^(-15).

We can determine the precision based on the number of iterations using the curves:

For n = 4, we observe a precision of 10^(-3).
For n = 8, we observe a precision of 10^(-4).
For n = 16, we observe a precision of 10^(-6).
For n = 32, we observe a precision of 10^(-11).
For n = 64, we observe a precision of 10^(-32).
We have now modified our program to plot the maximum error for sine and cosine for multiple values of n.

By using the files generated by the quali() function and the Octave script located in the directory [V2-VirguleFloat_ErrMax/script2.m](https://github.com/ahmetkurklu/cordic-implementation/blob/main/V2-VirguleFloat_ErrMax/script2.m), we plot the maximum linear error and the maximum logarithmic error.

![image](https://github.com/ahmetkurklu/cordic-implementation/assets/24780090/4b73c417-e95f-4102-82b2-af8e97a6b6bd)

*Maximum linear cosine/sine error*

![image](https://github.com/ahmetkurklu/cordic-implementation/assets/24780090/c6b56108-b530-43cc-a139-68e911f08ae1)

*Maximum logarithmic cosine/sine error*

The linear curve on the maximum error confirms that as the number of iterations increases, the error decreases. Using the logarithmic curve, we observe that after a certain point, the error becomes constant. This indicates that it is unnecessary to perform more iterations after surpassing this point, which occurs at n = 52.

## II.3. Fixed-Point Model in C
The principle of the fixed-point model is to replace floating-point numbers with integers and perform only addition, subtraction, and shifts operations. To achieve this, we need to define coefficients that will serve us to:

* Obtain integers by multiplying our floating-point numbers by the coefficient.
* Obtain floating-point numbers by dividing our integers by the coefficient.

Assuming that the coding is done on 16-bit integers: The values of cosine and sine are contained between -1 and 1, so we have a step size of 2. We know that 16 bits allow us to store 65,536 numbers. If we divide 16 bits by 65536, we get 65536/2 = 32,768. We round this value to 32,000 to ensure that we stay within the interval. The coefficient for cosine and sine is therefore 32000.

The values of the angles are assumed to be between -90 and 90 degrees, but during testing with the floating-point model, we noticed intermediate values exceeding the angles given in the subject. Therefore, we will assume that the angles vary between -120 and 120 degrees. By applying the same reasoning, we obtain 65536/240 = 273. We round this value to 270. The coefficient for our angles is therefore 270.

With the determined coefficients, we obtain a step size of 1/32000 = 3.125e-5 for cosine and sine, and a step size of 1/270 = 3.703e-3 for the angles. We can estimate a precision of 1e-5, which is close to the case with n = 16 for the floating-point model. Therefore, we assume that we need 16 iterations.

To facilitate the encoding and decoding, we write two functions: fp2fix() and fix2fp(), we also write a new cordic() function. The complete code of the fixed-point model can be found in the directory [V3-VirguleFixe_ErrCosSin/main.c](https://github.com/ahmetkurklu/cordic-implementation/blob/main/V3-VirguleFIxe_ErrCosSin/main.c).

We test this new function for an angle of 60 degrees and 16 iterations. 

* cos(phi) = 0.499937
* sin(phi) = 0.866031

Once again, we observe that the values are close to the values given in the subject, indicating that the function is working correctly.

## II.3.1. Qualification

By following the same approach as for the floating-point method, we obtain the following curves:

![image](https://github.com/ahmetkurklu/cordic-implementation/assets/24780090/b8048387-167e-467e-b436-d9141d9f0d2e)

*Error of cosine and sine for n = 4*

![image](https://github.com/ahmetkurklu/cordic-implementation/assets/24780090/d5f6b2fa-7bd8-4e29-a75f-92757a47192e)

*Error of cosine and sine for n = 8*

![image](https://github.com/ahmetkurklu/cordic-implementation/assets/24780090/1f6fe7c6-8f1b-4f61-a5dc-31adfb793a15)

*Error of cosine and sine for n = 16*

![image](https://github.com/ahmetkurklu/cordic-implementation/assets/24780090/e640f624-f86f-4f40-8ae7-13cdf324315c)

*Error of cosine and sine for n = 32*

![image](https://github.com/ahmetkurklu/cordic-implementation/assets/24780090/c044c1ae-3e88-4866-9b02-0488929a6169)

*Error of cosine and sine for n = 64*


We observe, as with the floating-point method, that the curves are symmetric, and some of them are symmetrical. This symmetry is interrupted this time for n = 32. We also notice that the error decreases between n = 4 and n = 16, and then increases again between n = 16 and n = 64. The increase in error after a certain number of iterations can be explained by the type we are using. We are using shorts, and when the precision increases after n = 16, we need more digits after the decimal point. However, with the chosen coefficients, we cannot store these numbers, which increases the error. Regarding the precision, we observe that for n = 4, 8, and 16, the precision is identical to the floating-point model, but beyond 10^-5, the precision no longer decreases. We can conclude that the fixed-point model is less precise.

![image](https://github.com/ahmetkurklu/cordic-implementation/assets/24780090/39961b42-65cf-4d83-8ede-42edfa283301)

*Maximum linear cosine/sine error*

![image](https://github.com/ahmetkurklu/cordic-implementation/assets/24780090/d4c7554a-dfe1-4cd6-a924-4f8f0252e0f2)

*Maximum logarithmic cosine/sine error*

We notice from the linear and logarithmic maximum error curves that after approximately 15 iterations, the error stabilizes. However, when we exceed 30 iterations, the error becomes very large. Therefore, we conclude that it is unnecessary to exceed 16 iterations.

## II.4. Behavioral VHDL Model

![image](https://github.com/ahmetkurklu/cordic-implementation/assets/24780090/521f2583-5ab8-4055-a735-06ab4ea5cbc3)

The translation of this RTL automaton into VHDL, as well as the testbench to perform the following tests, are available in the [CORDIC](https://github.com/ahmetkurklu/cordic-implementation/tree/main/CORDIC) directory.
We will now test our VHDL code with 2 angles and compare the obtained values with our fixed-point model.
For an angle of 60°, the following timing diagrams are obtained using gtkwave:


![image](https://github.com/ahmetkurklu/cordic-implementation/assets/24780090/02aff81b-1e93-455a-8609-d2dd28388ab9)

If we convert the values of r1 and r2 back to floating-point using our coefficient, we obtain r1 = 15998/32000 = 0.4999375 and r2 = 27713/32000 = 0.86603125.

We notice that the values obtained from the VHDL code and the fixed-point model are identical. Now, let's test for an angle of 40°. We obtain the following timing diagrams:

![image](https://github.com/ahmetkurklu/cordic-implementation/assets/24780090/3843f7d6-21b0-4554-8d74-7c5b872eebdd)

If we convert the values of r1 and r2 to floating point using our coefficient, we obtain r1 = 24512/3200 = 0.766 and r2 = 20569/3200 = 0.64278125.

We obtained the same values again with the VHDL code and the fixed-point model. The VHDL code is functioning correctly.
Now, we will proceed to perform the synthesis of the VHDL code using Quartus.

![image](https://github.com/ahmetkurklu/cordic-implementation/assets/24780090/88167276-5448-4d03-9818-c3d4be0f2f1a)

Using the synthesis report generated by Quartus, we observe that we are using 57 pins, 100 registers, and a total of 311 logic elements. For the maximum operating frequencies, we obtain the following results:

![image](https://github.com/ahmetkurklu/cordic-implementation/assets/24780090/18d7b3f3-b155-4d8f-87c8-72721c574f1b)

![image](https://github.com/ahmetkurklu/cordic-implementation/assets/24780090/27f97ace-7276-4f4c-83d2-f0c173fcd02d)

We have 17 rising edges between the start signal transitioning to 1 and the display of the results on r1 and r2. For the case where Fmax = 176.96 MHz, we have a period of 5.65 ns, resulting in a computation time of 5.65 * 17 = 96.05 ns. For the case where Fmax = 160.96 MHz, we have a period of 6.21 ns, resulting in a computation time of 6.21 * 17 = 105.57 ns.

## II.5. Architectural Model in VHDL

We obtain from the previous VHDL model the following operational and control units.

![image](https://github.com/ahmetkurklu/cordic-implementation/assets/24780090/7215e38f-0e46-466b-befa-04990e371c87)

![image](https://github.com/ahmetkurklu/cordic-implementation/assets/24780090/f8454d39-c1e0-4a09-87f9-d572daea82cb)

## III. Conclusion

This lab allowed us to understand the steps involved in implementing an algorithm on hardware. We learned the importance of starting with a C code in floating-point format and converting it to fixed-point format to simplify its implementation in VHDL. We also saw the significance of quantization, which helped us realize that exceeding a certain number of iterations was unnecessary. Ultimately, by following this approach, we realized that VHDL implementation is just a small part of the overall hardware implementation process.

