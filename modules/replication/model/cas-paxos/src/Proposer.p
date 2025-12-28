machine Proposer {
    var timestamp: int;
    var f: tChangeFunction;
    var value: int;

    var broadcast: tReliability;
    var acceptors: set[Acceptor];
    var accept_acks: set[Acceptor];
    var prepare_acks: set[Acceptor];

    var client: set[Client];

    start state Init {
        entry(setup: (broadcast: tReliability, acceptors: set[Acceptor])) {
            broadcast = setup.broadcast;
            acceptors = setup.acceptors;
            timestamp = 0;
            goto Wait;
        }
   }

    state Wait {
        entry {
            prepare_acks = default(set[Acceptor]);
            accept_acks = default(set[Acceptor]);
        }

        on eChangeRequest do (request: tChangeRequest) {
            f = request.value;
            client += (request.client);
            goto Prepare;
        }
       

    }

    state Prepare {
        entry {
            timestamp = timestamp + 1;
            Broadcast(broadcast, acceptors, ePrepareRequest, (proposer = this, timestamp = timestamp));
        }

        on ePrepareResponse do (response: tPrepareResponse) {
            if (response.confirmed) {
                prepare_acks += (response.acceptor);
                if (sizeof(prepare_acks) >= sizeof(acceptors) / 2 + 1) {
                    goto Accept;
                }
            } else {
                // Tell client it failed by returning a non-modified state.
                Broadcast(broadcast, client, eChangeResponse, (proposer = this, client = client[0], value = value));
                goto Wait;
            }
        }
    }

    state Accept {
        entry {
            // Apply f to state
            value = f;
            Broadcast(broadcast, acceptors, eAcceptRequest, (proposer = this, timestamp = timestamp, value = value));
        }

        on eAcceptResponse do (response: tAcceptResponse) {
            if (response.value == value) {
                accept_acks += (response.acceptor);
                if (sizeof(accept_acks) >= sizeof(acceptors) / 2 + 1) {
                    Broadcast(broadcast, client, eChangeResponse, (proposer = this, client = client[0], value = value));
                    goto Wait;
                }
            }
        }
    }
}

