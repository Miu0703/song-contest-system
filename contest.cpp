#include "contest.h"

std::ostream &operator<<(std::ostream &out, Participant const &p) {

// [Shantou/Song_Shantou/175/Singer_Shantou]
    return out << '[' + p.city() + '/' + p.song() + '/' + std::to_string(p.timeLength()) + '/' + p.singer() + ']';
}

std::ostream &operator<<(std::ostream &out, Voter const &p) {

// <Shantou/Regular>
    out << '<' << p.city() << '/';
    if (p.voterType() == 0) out << "All";
    if (p.voterType() == 1) out << "Regular";
    if (p.voterType() == 2) out << "Judge";
    out << '>';
    return out;
}

std::ostream &operator<<(std::ostream &out, MainControl const &m) {
/*
  displays the main features of the system.
  All the display is between curly brackets ({ in the first line, then continue
  to next line, then all the relevant data, then } in the last line.
  first line after { displays the phase - can be one of Registration or Contest or Voting.
  if phase is Registration, then all Registered participant are displayed
  sorted regarding city name. each city in a different line, in the same format
  as operator<< to a participant.
  if phase is Contest, nothing more is displayed.
  if phase is Voting, then all votes (both Regular and Judge) are displayed,
  in respect to all cities of registered participants.
  each city in a different line. The cities are sorted in respect to city name.

  example :
{
Registration
[Guangzhou/Song_Guangzhou/172/Singer_Guangzhou]
[Shanghai/Song_Shanghai/170/Singer_Shanghai]
[Shantou/Song_Shantou/175/Singer_Shantou]
[Shenzhen/Song_Shenzhen/180/Singer_Shenzhen]
}

  another example relevant to Voting phase - explanation will follow later :
{
Voting
Guangzhou : Regular(6) Judge(12)
Shanghai : Regular(1) Judge(10)
Shantou : Regular(1) Judge(0)
Senzhen : Regular(0) Judge(8)
}

*/
    out << '{' << std::endl;
    std::string cityTmp[1000];
    int sortTmp[1000];
    for (int i = 0; i < m.getParticipantsNums(); i++) {
        cityTmp[i] = m.getParticipants()[i]->city();
        sortTmp[i] = i;
    }
    for (int i = 0; i < m.getParticipantsNums(); i++)
        for (int j = 0; j < i; j++)
            if (cityTmp[i] < cityTmp[j]) {
                int tmp = sortTmp[i];
                sortTmp[i] = sortTmp[j];
                sortTmp[j] = tmp;
            }
    if (m.getSystemPhase() == Registration) {
        out << "Registration" << std::endl;
        for (int i = 0; i < m.getParticipantsNums(); i++)
            out << *m.getParticipants()[sortTmp[i]] << std::endl;
    } else if (m.getSystemPhase() == Voting) {
        out << "Voting" << std::endl;
        for (int i = 0; i < m.getParticipantsNums(); i++) {
            out << m.getParticipants()[sortTmp[i]]->city() + " : Regular(" << m.result[sortTmp[i]][0] << ") Judge("
                << m.result[sortTmp[i]][1]
                << ")" << std::endl;
        }
    } else {
    }
    out << '}' << std::endl;
    return out;
}

void Participant::update(std::string songName, int duration, std::string singerName) {
    // because it is NOT possible to change
    // the city name. if song name is "" then
    // the old name stays. the same is with the singer name.
    // if time length is 0 then the old one stays.
    // update can be done only if p1 is not registered, otherwise
    // nothing is done.
    if (registeredState != Registration) return;
    if (!songName.empty())
        this->songName = songName;
    if (!singerName.empty())
        this->songName = singerName;
    if (duration != 0)
        this->duration = duration;
    this->singerName = singerName;
}

MainControl &MainControl::operator+=(Participant &p) {
//        the registration will not succeed in the following cases :
//        1. The current phase is not Registration.
//                phase of the system is initialized by default to Registration.
//                Then it can be changed by setPhase method, e.g.
//                contest.setPhase(Contest);
//        contest.setPhase(Voting);
//        2. we've already reached to the max allowed number of participants (default is 26).
//        3. There is already a registered participant from the relevant city.
//        4. Name of city is "" or name of song or singer is "" or
//                                                           time length is greater than the max time allowed. default of max time
//        allowed is 180 (sec).
//        if registration fails, then just ignore it without anything else and
//        continue to the next operation in the program.
//                in this example p3 can not be registered bacause its time length 190
//        is greater than the maximum allowed.
    if (systemPhase != Registration) return *this; // 1
    if (participantsNums == maxNum) return *this; //2
    for (int i = 0; i < participantsNums; i++)
        if (p.city() == participants[i]->city()) return *this; //3
    if (p.city().empty() || p.song().empty() || p.singer().empty() || p.timeLength() > maxLength)
        return *this; //4
    p.updateRegistered(true);
    participants[participantsNums++] = &p;
    return *this;
}

MainControl &MainControl::operator-=(Participant &p) {
    if (systemPhase != Registration) return *this;
    bool found = false;
    for (int i = 0; i < participantsNums; i++)
        if (participants[i]->city() == p.city()) {
            for (int j = i; j < participantsNums - 1; j++)
                participants[j] = participants[j + 1];
            found = true;
            break;
        }
    if (!found) return *this;
    participantsNums--;
    p.updateRegistered(false);
    return *this;
}

bool MainControl::legalParticipant(Participant p) {
/* the operation legalParticipant checks if all the following exists :
   the names of the city and the song and the singer are not empty "",
   as well as time length of the song is not big than the max allowed time length
   for any song. this max allowed value is determined when defining the
   system object. default is 180 (sec).
*/
    if (p.city().empty() || p.song().empty() || p.singer().empty()) return false;
    return p.timeLength() <= maxLength;
}

void MainControl::setPhase(Phase newPhase) {
    if (systemPhase == Registration && newPhase == Contest) systemPhase = Contest;
    if (systemPhase == Registration && newPhase == Contest) systemPhase = Voting;
    systemPhase = newPhase;
}

MainControl::~MainControl() {
    for (int i = 0; i < participantsNums; i++) participants[i] = nullptr;
}

bool MainControl::participate(std::string cityName) {
    for (int i = 0; i < participantsNums; i++)
        if (participants[i]->city() == cityName) {
            return participants[i]->isRegistered();
        }
    return false;
}

MainControl &MainControl::operator+=(const Vote &v) {
    if (systemPhase != Voting) return *this;
    if (v.from->voterType() == Regular) {
        if (v.from->city() == v.names[0]) return *this; //a voter can not vote for his own city.
        int pos = -1;
        for (int i = 0; i < participantsNums; i++)
            if (participants[i]->city() == v.from->city()) {
                if (!participate(participants[i]->city())) return *this;
                pos = i;
            }
        if (pos == -1 || !participate(v.names[0])) return *this; //can vote for any other city, in condition that both
        // his city and the city to vote for are registered and participate.

        if (v.from->timesOfVotes() >= maxTimes) return *this;
        for (int i = 0; i < participantsNums; i++)
            if (participants[i]->city() == v.names[0]) {
                result[i][0]++;
                break;
            }
        v.from->setNums(v.from->timesOfVotes() + 1);
    } else if (v.from->voterType() == Judge) {
        const int rewards[] = {12, 10, 8, 7, 6, 5, 4, 3, 2, 1};
        for (int k = 0; k < v.namesNums; k++) {
            if (v.from->city() == v.names[k]) return *this; //a voter can not vote for his own city.
            int pos = -1;
            for (int i = 0; i < participantsNums; i++)
                if (participants[i]->city() == v.from->city()) {
                    if (!participate(participants[i]->city())) return *this;
                    pos = i;
                }
            if (pos == -1 || !participate(v.names[k]))
                return *this; //can vote for any other city, in condition that both
            // his city and the city to vote for are registered and participate.

            if (v.from->timesOfVotes() >= maxTimes) return *this;
            for (int i = 0; i < participantsNums; i++)
                if (participants[i]->city() == v.names[k]) {
                    result[i][1] += rewards[k];
                    break;
                }
        }
    }
    return *this;


/*
   a Judge voter can vote only ONCE, no more. all additions votings of his are
   just ignored without furthere notification.
   in contradiction to a regular voter, he can give a list of 1 to 10 different city names.
   the points given to the cities are according to the order they are mentioned :
   first city gets 12 points, second 10 points, third 8 points and so on.
   if there are less than 10 cities, then only the given cities get points, starting from 12,
   according to their order.
   if there are cities that are not registered in the system, they are ignored.
   in any case, the points to every city are given regarding its absolute order number,
   no matter if previous cities in the list are not legal.
   remark - we assume that the list for a Judge voter does not have cities that appear
   more than once. No need to check this.
*/
}

MainControl::MainControl(int maxLength, int maxNum, int maxTimes) : maxLength(maxLength), maxNum(maxNum),
                                                                    maxTimes(maxTimes) {}

MainControl::MainControl() {}

MainControl::Iterator MainControl::begin() const {
    return Iterator(this, 0);
}

MainControl::Iterator MainControl::end() const {
    return Iterator(this, participantsNums);
}

std::string MainControl::operator()(int id, VoterType vt) {
    int sortTmp[1000];
    for (int i = 0; i < getParticipantsNums(); i++)
        sortTmp[i] = i;
    if (id > getParticipantsNums()) return "";
    if (vt == Regular) {
        for (int i = 0; i < getParticipantsNums(); i++)
            for (int j = 0; j < i; j++)
                if ((result[sortTmp[i]][0] == result[sortTmp[j]][0] &&
                     participants[sortTmp[i]]->city() > participants[sortTmp[j]]->city()) ||
                    result[sortTmp[i]][0] > result[sortTmp[j]][0]) {
                    int tmp = sortTmp[i];
                    sortTmp[i] = sortTmp[j];
                    sortTmp[j] = tmp;
                }
    }else if (vt == Judge){
        for (int i = 0; i < getParticipantsNums(); i++)
            for (int j = 0; j < i; j++)
                if ((result[sortTmp[i]][1] == result[sortTmp[j]][1] &&
                     participants[sortTmp[i]]->city() > participants[sortTmp[j]]->city()) ||
                    result[sortTmp[i]][1] > result[sortTmp[j]][1]) {
                    int tmp = sortTmp[i];
                    sortTmp[i] = sortTmp[j];
                    sortTmp[j] = tmp;
                }
    }else{
        for (int i = 0; i < getParticipantsNums(); i++)
            for (int j = 0; j < i; j++)
                if ((result[sortTmp[i]][0] + result[sortTmp[i]][1] == result[sortTmp[i]][0] + result[sortTmp[j]][1] &&
                     participants[sortTmp[i]]->city() > participants[sortTmp[j]]->city()) ||
                        result[sortTmp[i]][0] + result[sortTmp[i]][1] > result[sortTmp[i]][0] + result[sortTmp[j]][1]) {
                    int tmp = sortTmp[i];
                    sortTmp[i] = sortTmp[j];
                    sortTmp[j] = tmp;
                }
    }
    return participants[sortTmp[id - 1]]->city();
}

VoterType Voter::voterType() const {
    return type;
}

Voter::Voter(const std::string &cityName, VoterType type) : type(type), cityName(cityName) {}

Voter::Voter(const std::string &cityName) : cityName(cityName) {}

int Voter::timesOfVotes() const {
    return nums;
}

void Voter::setNums(int nums) {
    Voter::nums = nums;
}


Vote::~Vote() {
    from = nullptr;
}

Vote::Vote(Voter &from, std::string p1, std::string p2, std::string p3, std::string p4,
           std::string p5, std::string p6, std::string p7, std::string p8, std::string p9,
           std::string p10) {
    this->from = &from;
    names[namesNums++] = p1;
    if (!p2.empty()) names[namesNums++] = p2;
    if (!p3.empty()) names[namesNums++] = p3;
    if (!p4.empty()) names[namesNums++] = p4;
    if (!p5.empty()) names[namesNums++] = p5;
    if (!p6.empty()) names[namesNums++] = p6;
    if (!p7.empty()) names[namesNums++] = p7;
    if (!p8.empty()) names[namesNums++] = p8;
    if (!p9.empty()) names[namesNums++] = p9;
    if (!p10.empty()) names[namesNums++] = p10;
}

const Participant &MainControl::Iterator::operator*() const {
    std::string cityTmp[1000];
    int sortTmp[1000];
    for (int i = 0; i < mainControl->participantsNums; i++) {
        cityTmp[i] = mainControl->participants[i]->city();
        sortTmp[i] = i;
    }
    for (int i = 0; i < mainControl->participantsNums; i++)
        for (int j = 0; j < i; j++)
            if (cityTmp[i] < cityTmp[j]) {
                int tmp = sortTmp[i];
                sortTmp[i] = sortTmp[j];
                sortTmp[j] = tmp;
            }
    return *mainControl->participants[sortTmp[index]];
}

MainControl::Iterator &MainControl::Iterator::operator++() {
    ++index;
    return *this;
}

MainControl::Iterator MainControl::Iterator::operator++(int) {
    Iterator result = *this;
    ++*this;
    return result;
}

bool MainControl::Iterator::operator==(const MainControl::Iterator &it) const {
    return index == it.index;
}

bool MainControl::Iterator::operator!=(const MainControl::Iterator &it) const {
    return !(*this == it);
}

bool MainControl::Iterator::operator<(const MainControl::Iterator &it) const {
    return index < it.index;
}

