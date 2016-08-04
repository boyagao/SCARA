Author: Boya Gao
#Instructions: #

	`make`	   - compiles scara.cpp but doesn't run
	`make all`   - compiles, run, clean everything
	`make run`   - compiles and runs scara
	`make clean` - removes all output files and executable
	
	
#Description:#

	Displays a 3-link SCARA robot arm based on Angel's chapter 8 definition
	of his robot arm. The joints to be manipulated can be selected by right
	clicking in the window to bring up a menu of joint options. Joints one
	and two rotate while joint three translates up and down. To perform
	rotate/translation, press + or -. Additionally, camera angle can be
	changed through its theta or phi given this formula for eye:

	point4  eye( sin(theta)*cos(phi),
        	     sin(theta)*sin(phi),
         	     cos(theta), 1.0 );

	Simple instructions to execute all mentioned changes are as follows:


	--------------------INSTRUCTIONS-------------------- 
	Right click with mouse to select joint (default is Joint One) 
	Press + to rotate joint clockwise or move up 
	Press - to rotate joint counterclockwise or move down 
	Press J or SHIFT+J to decrease/increase Phi component of the angle 
	Press K or SHIFT+K to decrease/increase Theta component of the angle 
