#ifndef __SOURCE__
#define __SOURCE__
class Source {

private:
    int val;

    Source(int val);

public:
    static int id;

    static void reset_id();

    static Source* generate(int val);    

    int use();
};

#endif