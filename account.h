
typedef long Userid;
struct User{
    char name[20];
    Userid id;
    char password[20]; // sha-1
};

