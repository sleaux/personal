machine Basic {
    start state Init {
        entry {
            var proposer: Proposer;
            var acceptors: set[Acceptor];
            var client: Client;
            acceptors += (new Acceptor(Reliable));
            proposer = new Proposer((broadcast = Reliable, acceptors = acceptors));
            client = new Client((broadcast = Reliable, proposer = proposer, value = 99));
        }
    }
}

test testBasic [main = Basic]:
    assert HistoryIsLinearizable, ChangeSucceeded in (union CasPaxos, {Basic});
