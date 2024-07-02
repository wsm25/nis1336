#include "task.h"
#include <vector>

struct Filter{};

class interface{
public:
    void insert(Task);
    std::vector<Task&> select(Filter);
    void edit(Id, Task);
};

