typdef enum {            // Set of states enumerated
       IDLE_STATE,
       COIN_INSERTED_STATE,
       OPTION_SELECTED_STATE,
       MAX_STATES
       } state_e;

typedef enum {            // Set of events enumerated
    INSERT_COIN_EVENT,
    SELECT_OPTION_EVENT,
    COFFEE_READY_EVENT,
    MAX_EVENTS
    } event_e;

state_e state = IDLE_STATE;    // Starting state
state_e next_state;
event_e event;

while(1)            // When event occurs, event handler does some stuff
{
    event = read_event();
    if (state == IDLE_STATE)
    {
        if (event == START_PRESS)
        {
            next_state == game_start_event_handler();
        }
    else if (state == GAME_STARTED_INSERTED_STATE)
    {
        if (event == WHACKED)
        {
            next_state == whacked_event_handler();
            score = 1;
        }
    else if (state == MISSED)
    {
        if (event == GAME_STARTED_INSERTED_STATE)
        {
            next_state == missed_event_handler();
            score = -1;
        }
    }
    state = next_state;

