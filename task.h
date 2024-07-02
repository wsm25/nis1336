struct Time{};
typedef unsigned long Id;

enum Priority{
    Default,
    Low,
    Mid,
    High,
};

enum Status{
    Unfinished,
    Finished,
    Abort,
};



struct Tag{char name[256];};
struct Date{};
class Dates{
public:
    bool is_today(const Date&);
};

struct Task{
    Id id;
    char name[256]; // as file name
    Time departure, arrival;
    Priority priority;
    Tag tag;
    Dates dates;
    Status status;
    char content[65536];
};

