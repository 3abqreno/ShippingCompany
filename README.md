# ShippingCompany
Shipping company cargo simulator using c++ OOP and data structures

## porject idea
The project revolves around a shipping company that has cargo and deleivery trucks and you have to optimize their usage to get minimize delivery time and distance moved by every truck

### Data structures 
we implement a min heap as a priority queue to move the premium cargo first based on an equation on delivery time and cost and distance, we also used a queue to simulate normal and special cargo.

### Events

there was a number of events that the program had to handle based on the given input file
_ ready cargo with various kinds
_ cancel order
_order promotion

### program output
the program runs on the given you the statistics of the run for example cargo delivery time and the truck id it was shipped on, also it gives you active time and utilization of the trucks

![image](https://user-images.githubusercontent.com/102177769/222241522-240336ab-662d-4df9-9768-e2dbaaba5a81.png)


### Program modes
the program had 3 modes that it can run on
1- interactive mode and in this mode you press space or any key to increment the time

2- step by step mode and in this mode time increments every second

3- silent mode and in this mode it runs the program on the given input and gives you the statistics in the output file with out showing you the whole simulation
