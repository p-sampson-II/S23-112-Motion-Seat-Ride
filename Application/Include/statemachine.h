namespace state {
    struct Idle {

    };
    struct Active {

    };
    struct Fault {

    };
    struct Command {

    };
}

class StateMachine {
public:
    StateMachine();
    ~StateMachine();

private:
    SMImpl *impl;
};