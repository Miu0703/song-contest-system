# song-contest-system
In this HW we deal with a song contest between cities in China. Each city is represented by a song. There are rules how to registered to the contest, how to participate, how the winner is selected etc. We’ll design and implement a system to support all relevant entities (structs/classes) and operations.

Part A :
The file test1.cpp is given. We have the following entities :

1. class Participant

represents a potential participant (city) in the contest. Characteristics are : name of city, name of song, name of singer (or group), time duration of the song (in seconds , int).
As long as still NOT formally enrolled to the contest, can update the values, except the name of city.

2. class Voter

represents a voter in the contest. Each voter belongs to a certain city. A voter can not vote for his own city. The relevant cities of voters are those cities who do participate in the contest. A city that does not participate in the contest can not vote.

There are 2 types of voters :
  1. Regular – This is a regular voter that in each voting operation he vote for one city only.
Each voting operation equals 1 point. A regular voter can vote more than 1 time, but no more than a limit that is defined as a parameter in system of the contest.
  2. Judge – This kind of a voter is a professional voter that votes according to professional
characteristics. He can vote only 1 time. In his voting he gives not more than 10 different
cities (without his own city, of course) that gets the points of 1 2 3 4 5 6 7 8 10 12.

3. struct Vote

 This struct is in use when doing the voting operation. It gives the details of who is voting and for who is this voting. Needed to have here only constructor and destructor. No need any other
methods.

4. class MainControl

This class represents the management of the contest. It handles the registration of the candidates and checking of their legality. Moreover, it handles the voting process, announces the results etc.

Registration of a potential participant is done via the operation +=
A voting operation is also done via the operation +=.

The contest control can be in 1 of the following 3 phases :
Registration : in this phase the participants are formally registered or may even cancel their
registration.
Contest : In this phase all participants perform their song on stage. This phase does not have
any meaningful operation in our program.
Voting : In this phase the voting process is done and the results are announced.

In Part A you need to do the following :
  1. Read carefully the program in test1.cpp and its output in test1_out.txt.
Verify to understand the use of all entities appearing there.
  2. Given the file Contest.h, complete it with all necessary declarations/definitions
So that it will be fully compatible with the program in test1.cpp.
Must NOT use here templates, nor STL, nor inheritance.
No need to add anything that is not required from what appears in test1.cpp.
Concerning error conditions, illegal parameters etc – in this Part A we do NOT
deal with all this, do not return any error status and do not throw any exception.
Any operation that has any problem will not be operated at all. We assume that
such case is only local to this operation and does not prevent from continuing
to operate the following code in the program.
  3. Write the file Contest.cpp (if needed).
  4. Compile/Run the program and verify that no errors/warnings at all.
In addition make sure that the output is identical to test1_out both in values and
format.

Part B :
In this Part we will extend Part A.


We will use template. In addition, using STL is also allowed here.

5. Define a generic function (template) whose name is get that finds the i-th most big
element in a container. The container is given by begin/end values of an iterator,
As learned in class. Notice that the order of iterating as supplied by the iterator does not necessarily follow the value order between elements in the container. Assume that all elements are different. Must not change the container.
The function returns an iterator to the i-th biggest element. For example, i==1 means
the most big element. If can not find such an element (illegal i / too big I etc) then the result is an iterator to one place after the last element.

No special requirement here concerning complexity/efficiency.


6. We have the following additional tests in continuation to Part A :

TEST("1.7.1")
 MainControl::Iterator i;
 for (i = contest.begin(); i<contest.end(); ++i)
 cout << *i << endl;
// we get here the display of all the participants that are registered in the
// contest. They are displayed sorted by state name, from small to big.

TEST("1.7.2")
for (i = contest.begin(); !(i==contest.end()); ++i)
 cout << *i << endl;
 // the same as 1.7.1
 
TEST("1.7.3")
// --i; // compilation error. operator -- does not exist for i.
Add to the class MainControl all the required additions in order to support these tests.

7. In continuation to Part A, in the end of the voting process, we want to support the
following operations :

TEST("1.17.1")
 cout << contest(1, Regular) << endl;
/* We get here displayed the name of the city that won 1st place according to Regular
votes */
/* if 2 cities have the same num of points, then first comes the city whose name is
bigger. i.e. if Shantou and Shanghai got together the same max num points, then place
1 is for Shantou and place 2 is for Shanghai. */
 cout << contest(2, Regular) << endl;
/* the city that won 2nd place according to Regular votes */
 cout << contest(3, Regular) << endl;
/* the city that won 3rd place according to Regular votes */
 cout << contest(4, Regular) << endl;
/* the city that won 4th place according to Regular votes */
 cout << contest(5, Regular) << endl;
/* the city that won 5th place according to Regular votes */
/* if no such winner, then “” is displayed */

TEST("1.17.2")
/* the same as TEST 1.17.1 but this time according to Judge votes */
 cout << contest(1, Judge) << endl;
 cout << contest(2, Judge) << endl;
 cout << contest(3, Judge) << endl;
 cout << contest(4, Judge) << endl;
 cout << contest(5, Judge) << endl;
 
 TEST("1.17.3")
/* the same as TEST 1.17.2 but this time according to All votes, which means
points==(Regular points)+(Judge points) */
 cout << contest(1, All) << endl;
 cout << contest(2, All) << endl;
 cout << contest(3, All) << endl;
 cout << contest(4, All) << endl;
 cout << contest(5, All) << endl;
 
 Add all what is needed in MainControl in order to support these tests.
 The operation contest(i , Voter Type) must be implemented using the generic function
(template) get (section 5).
 Assume that the tests are done in the end of the voting process.
 If from any reason can not get the name of a city then we get “”.
The files test2.cpp test2_out.txt demonstrate the above tests. Run the program and verify to have a complete correct program, without any warning/error. Specifically verify to have correct output results, both in values and format.

Compilation/Link

Part A :

g++ -o test1.exe -std=c++11 -Wall -pedantic-errors -Werror -DNDEBUG test1.cpp contest.cpp

Part B :

g++ -o test2.exe -std=c++11 -Wall -pedantic-errors -Werror -DNDEBUG test2.cpp contest.cpp

Notice to the following :
check your program (Both output values and format). Put special care to memory leak (use valgrind).

use assert when is needed

add documentation so that the code is reasonably readable. 
