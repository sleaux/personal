spec HistoryIsLinearizable observes eAcceptRequest, eAcceptResponse {
    var history: map[Acceptor, map[int, int]];

    start state Init {
        entry {
            history = default(map[Acceptor, map[int, int]]); 
        }

        on eAcceptRequest do (payload: tAcceptRequest) {

        }

        on eAcceptResponse do (payload: tAcceptResponse) {
            if (!(payload.acceptor in history)) {
                history[payload.acceptor] = default(map[int, int]);     
            }
            history[payload.acceptor][payload.timestamp] = payload.value;
            assert checkHistory(history);
        }           
    }

    fun checkHistory(history: map[Acceptor, map[int, int]]): bool {
        // There must be some total order of the timestamps of every
        // operation such that:
        //  - Every operation that was responded to is included in the
        //    total order.
        //  - For each change function application that occurs after a response,
        //    the order of operations in the total order is the same as in
        //    each nodes.
        //  - The first successful operation must have its old value be
        //    the initial value.
        //  - Each operation that happens directly after another must have
        //    the prior operations new value match the subsequent operations
        //    old value.
        print format("{0}", history);


        return true;
    }
}
