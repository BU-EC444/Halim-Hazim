# Quest 4: Electronic Voting
Authors: Hazim Halim

Date: 2021-04-13
-----

## Summary

This quest impliments the use of these concepts: 
    1) IR Receiver and Transmitter
    2) UDP client
    3) Databases
    4) FSM
    
It requires me to implement a voting system using the mentioned concepts. An IR signal will be sent from a fob to the leader fob. The IR signal will contain the votes casted as well as the ID of the fob which casted the vote.

The leader fob will then send the results of the votes to a server which will handle two things, storing the data inside a database structure (in this quest, I used LevelDB), and send a UDP socket containing data in the DB to a web server which will then display it on a web page.

## Self-Assessment

### Objective Criteria

| Objective Criterion | Rating | Max Value  | 
|---------------------------------------------|:-----------:|:---------:|
| Objective One | 1 |  1     | 
| Objective Two | 1 |  1     | 
| Objective Three | 1 |  1     | 
| Objective Four | 1 |  1     | 
| Objective Five |0  |  1     | 
| Objective Six | 0|  1     | 
| Objective Seven | 1 |  1     | 


### Qualitative Criteria

| Qualitative Criterion | Rating | Max Value  | 
|---------------------------------------------|:-----------:|:---------:|
| Quality of solution | 3 |  5     | 
| Quality of report.md including use of graphics | 3 |  3     | 
| Quality of code reporting | 3 |  3     | 
| Quality of video presentation | 2|  3     | 

# Investigative Question:
List 5 different ways that you can hack the system (including influencing the vote outcome or preventing votes via denial of service)

## Answer: Weak points in the system: 

UDP connection: If someone knows the IP address of the server, they can send their own UDP socket to the same server and increase the number of votes for their preferred candidate.

IR Transmitter and Receiver: Since the fobs communicate the votes using IR, anyone can build a similar, unregistered fob which can be used to cast votes without officially being in the system.

Leader fob: Someone can spoof a fob as the leader fob. Since the leader fob is the one responsible for sending data to the DB, spoofing a leader fob can result in mass casting of votes which aren't from the registered fobs.

Database: As of now, there is no restriction on who can or can't write into the DB. So, by altering the data stored in the DB, someone can increase votes for their preferred candidate or delete votes for their rival.

Web page: Someone can hack the webpage to display false voting results despite the database proving otherwise.

## Answer: Overcoming these attacks:

Use a unique indentifier for each stage of the voting systems.

Use less wireless networks in the critical path of the systems such as the connection between the leader fob and the server.

Make a two way connection confirming the votes between the fobs to ensure that the fobs are also valid.


<center><img src="./images/ece444.png" width="25%" /></center>  
<center> </center>


## Supporting Artifacts

https://drive.google.com/file/d/1XatPKRyAyx-e_RtkqI1mujbYW6Be5WAk/view?usp=sharing
## Modules, Tools, Source Used Including Attribution
UART
Node.js
LevelDB
Socket.io


## References
https://www.freecodecamp.org/news/state-machines-basics-of-computer-science-d42855debc66/
https://en.wikipedia.org/wiki/Finite-state_machine
https://en.wikipedia.org/wiki/Bully_algorithm


-----
