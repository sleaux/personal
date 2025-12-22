type tChangeFunction = int;

type tPrepareRequest = (proposer: Proposer, timestamp: int);
event ePrepareRequest: tPrepareRequest;
type tPrepareResponse = (acceptor: Acceptor, timestamp: int, confirmed: bool);
event ePrepareResponse: tPrepareResponse;

type tAcceptRequest = (proposer: Proposer, timestamp: int, value: int);
event eAcceptRequest: tAcceptRequest;
type tAcceptResponse = (acceptor: Acceptor, timestamp: int, value: int);
event eAcceptResponse: tAcceptResponse;

type tChangeRequest = (client: Client, value: int);
event eChangeRequest: tChangeRequest;
type tChangeResponse = (proposer: Proposer, client: Client, value: int);
event eChangeResponse: tChangeResponse;

type tStore = (value: int);
event eStore: tStore;
