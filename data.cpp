#include "data.h"

void loadData(
    std::vector<Activity>& acts,
    std::vector<Room>& rooms,
    std::vector<std::string>& times,
    std::vector<Facilitator>& facs
) {
    // ----- Activities -----
    acts.clear();

    acts.push_back({ "SLA101A", 40,
        {"Glen", "Lock", "Banks"},
        {"Numen", "Richards", "Shaw", "Singer"},
        false, false });

    acts.push_back({ "SLA101B", 35,
        {"Glen", "Lock", "Banks"},
        {"Numen", "Richards", "Shaw", "Singer"},
        false, false });

    acts.push_back({ "SLA191A", 45,
        {"Glen", "Lock", "Banks"},
        {"Numen", "Richards", "Shaw", "Singer"},
        true,  false }); // requires Lab

    acts.push_back({ "SLA191B", 40,
        {"Glen", "Lock", "Banks"},
        {"Numen", "Richards", "Shaw", "Singer"},
        true,  false }); // requires Lab

    acts.push_back({ "SLA201", 60,
        {"Glen", "Banks", "Zeldin", "Lock", "Singer"},
        {"Richards", "Uther", "Shaw"},
        false, false });

    acts.push_back({ "SLA291", 50,
        {"Glen", "Banks", "Zeldin", "Lock", "Singer"},
        {"Richards", "Uther", "Shaw"},
        true,  false }); // requires Lab

    acts.push_back({ "SLA303", 25,
        {"Glen", "Zeldin"},
        {"Banks"},
        true,  true }); // Lab + Projector

    acts.push_back({ "SLA304", 20,
        {"Singer", "Uther"},
        {"Richards"},
        true,  false }); // Lab

    acts.push_back({ "SLA394", 15,
        {"Tyler", "Singer"},
        {"Richards", "Zeldin"},
        false, false });

    acts.push_back({ "SLA449", 30,
        {"Tyler", "Zeldin", "Uther"},
        {"Zeldin", "Shaw"},
        false, true }); // Projector

    acts.push_back({ "SLA451", 90,
        {"Lock", "Banks", "Zeldin"},
        {"Tyler", "Singer", "Shaw", "Glen"},
        true,  true }); // Lab + Projector

    // ----- Rooms -----
    rooms.clear();
    rooms.push_back({ "Beach 201", 18,  false, true });
    rooms.push_back({ "Beach 301", 25,  true,  true });
    rooms.push_back({ "Frank 119", 95,  true,  true });
    rooms.push_back({ "Loft 206", 55,   false, false });
    rooms.push_back({ "Loft 310", 48,   true,  false });
    rooms.push_back({ "James 325",110,  true,  true });
    rooms.push_back({ "Roman 201",40,   false, false });
    rooms.push_back({ "Roman 216",80,   true,  true });
    rooms.push_back({ "Slater 003",32,  true,  true });

    // ----- Times -----
    times = { "10 AM", "11 AM", "12 PM", "1 PM", "2 PM", "3 PM" };

    // ----- Facilitators -----
    facs = {
        {"Lock"}, {"Glen"}, {"Banks"}, {"Richards"}, {"Shaw"},
        {"Singer"}, {"Uther"}, {"Tyler"}, {"Numen"}, {"Zeldin"}
    };
}
