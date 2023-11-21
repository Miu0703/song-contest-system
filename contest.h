#ifndef CONTEST_H_
#define CONTEST_H_

#include <iostream>
#include <string>
#include <algorithm>

// it's allowed to perform here any using statements, according to needs.
// do NOT perform here : using namespace std;

//---------------------------------------------------

enum VoterType {
    All, Regular, Judge
};
enum Phase {
    Registration, Contest, Voting
};

//---------------------------------------------------

class Participant {
// relevant private members can be defined here, if necessary.
    std::string cityName, songName, singerName;
    int duration;
    bool registeredState = false;
public :
    Participant(const std::string &cityName, const std::string &songName, int duration, const std::string &singerName)
            : cityName(cityName), songName(songName), singerName(singerName), duration(duration) {}

    std::string city() const {
        return cityName;
    }

    std::string song() const {
        return songName;
    }

    std::string singer() const {
        return singerName;
    }

    int timeLength() const {
        return duration;
    }

    bool isRegistered() const {
        return registeredState;
    }

    void updateRegistered(bool newState) {
        registeredState = newState;
    }

    void update(std::string songName, int duration, std::string singerName);

// need to define here possibly c'tr and d'tr and ONLY methods that
// are mentioned and demonstrated in the test example that has been published.
// NO OTHER METHODS SHOULD APPEAR HERE.

// NO friend is allowed here.

};

std::ostream &operator<<(std::ostream &out, Participant const &p);


class Voter {
    //  relevant private members can be defined here, if necessary.
    VoterType type{Regular};
    std::string cityName;
    int nums = 0;

public:
    void setNums(int nums);

    int timesOfVotes() const;

    const std::string city() const {
        return cityName;
    }

    Voter(const std::string &cityName, VoterType type);

    Voter(const std::string &cityName);

    VoterType voterType() const;

    Voter &operator++() {
        ++nums;
        return *this;
    }


public :

// need to define here possibly c'tr and d'tr and ONLY methods that
// are mentioned and demonstrated in the test example that has been published.
// NO OTHER METHODS SHOULD APPEAR HERE.

// NO friend is allowed here.

};

std::ostream &operator<<(std::ostream &out, Voter const &p);

// -----------------------------------------------------------

struct Vote {
    Voter *from;
    std::string names[11]{};
    int namesNums{0};

// ALL is public here.
// need to define ONLY data members and c'tr and d'tr.
    Vote(Voter &from, std::string p1, std::string p2 = "", std::string p3 = "", std::string p4 = "",
         std::string p5 = "", std::string p6 = "", std::string p7 = "", std::string p8 = "", std::string p9 = "",
         std::string p10 = "");

    virtual ~Vote();
// NO NEED to define anything else.
};

// -----------------------------------------------------------


class MainControl {
    // relevant private members can be defined here, if necessary.
    int maxLength = 180, maxNum = 26, maxTimes = 5;
    Phase systemPhase = Registration;
    Participant *participants[1000]{};
    int participantsNums = 0;
    /* The same as defining with the default values
   MainControl contest(180, 26, 5);
   180 - max allowed time length (seconds) of a song
   26 - max allowed number of participants
   5 - max allowed times that any regular voter (not judge) can vote.
   other inner default initializations :
   empty system at the start - no participants, no votes,
   phase of Registration.
*/

public :
    class Iterator;

    Iterator begin() const;

    Iterator end() const;

    int result[1000][2]{};

    MainControl();

    MainControl(int maxLength, int maxNum, int maxTimes);

    virtual ~MainControl();

    Phase getSystemPhase() const {
        return systemPhase;
    }

    Participant *const *getParticipants() const {
        return participants;
    }

    int getParticipantsNums() const {
        return participantsNums;
    }

    MainControl &operator+=(Participant &p);

    MainControl &operator-=(Participant &p);

    MainControl &operator+=(const Vote &v);

    std::string operator()(int id, VoterType vt);

    void setPhase(Phase newPhase);

    bool legalParticipant(Participant p);

    bool participate(std::string cityName);
// need to define here possibly c'tr and d'tr and ONLY methods that
// are mentioned and demonstrated in the test example that has been published.
// NO OTHER METHODS SHOULD APPEAR HERE.

// Also it's allowed here to define friend.

};


class MainControl::Iterator {
    const MainControl *mainControl;
    int index;

    friend class mainControl;

public:
    const Participant &operator*() const;

    Iterator &operator++();

    Iterator operator++(int);

    bool operator==(const Iterator &it) const;

    bool operator!=(const Iterator &it) const;

    bool operator<(const Iterator &it) const;


    Iterator(const MainControl *mainControl, int index) : mainControl(mainControl), index(index) {}

    Iterator() {
        index = 0;
    }
};

std::ostream &operator<<(std::ostream &out, MainControl const &m);

// -----------------------------------------------------------

#endif
