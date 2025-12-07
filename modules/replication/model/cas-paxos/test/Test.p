

machine Basic {
    start state Init {
        entry {
            var proposer: Proposer;
            var acceptor: Acceptor;
            var client: Client;
            proposer = new Proposer(Reliable);
            acceptor = new Acceptor(Reliable);
            client = new Client(Reliable);
        }
    }
}
