The following steps must be followed for successful compilation of the program:

1) open the folder in VSCode
2) Go to main.c; go to terminal in the taskbar and click on “configure tasks”
3) In your tasks.json file, change "${file}" in line 11 to 
"${fileDirname}/main.c",
"${fileDirname}/drawutil.c", in a mac environment or to
"${fileDirname}/*.c", in a windows environment
4) Return to main.c and back to terminal in the taskbar where select Run Build Task
5) Once compilation is successful you can use the command: cat input.txt | ./main (or main.exe) to print on the terminal 
or use cat input.txt | ./main >output.txt to print in a separate file called “output.txt”

Refer to the platform specific .vscode zip file folders in case help is required.

Sample Input:
10
49, 49
20, 20
10, 10
70, 70
60, 60
65, 65
71, 71
75, 75
80, 80
81, 81
50, 50, 25
70, 70, 25

Output:
The first 100x100 screen will display the construction of the quad tree along with the points inputted in it.
The second 100x100 screen will show the Range Query - all points contained in circle one.
The third 100x100 screen will show the Traffic Query - smallest node of quad tree with maximum points.
The fourth 100x100 screen will show the Circle Intersection Query - the points that lie within the area of intersection of the 2 circles.
At last, we display the 3 closest points to the center of the 2nd circle in increasing order of its distance from the same.
