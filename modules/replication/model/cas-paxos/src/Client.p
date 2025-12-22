machine Client {
    var value: int;
    var proposer: set[Proposer];
    var broadcast: tBroadcast;

    start state init {
        entry(setup: (broadcast: tBroadcast, proposer: Proposer, value: int)) {
            broadcast = setup.broadcast;
            proposer += (setup.proposer);
            value = setup.value;
            goto Change;
        }
    }

    state Change {
        entry {
            Broadcast(broadcast, proposer, eChangeRequest, (client = this, value = value));
        }

        on eChangeResponse do (response: tChangeResponse) {

        }
    }
}

