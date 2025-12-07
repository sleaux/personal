type tBallot = int;
type tState = int;
type tChangeFunction = int;

type tPrepareRequest = (proposer: Proposer, ballot: tBallot);
event ePrepareRequest: tPrepareRequest;
type tPrepareResponse = (acceptor: Acceptor, confirmed: bool);
event ePrepareResponse: tPrepareResponse;

type tAcceptRequest = (proposer: Proposer, ballot: tBallot, new_state: tState);
event eAcceptRequest: tAcceptRequest;
type tAcceptResponse = (acceptor: Acceptor, confirmed: bool);
event eAcceptResponse: tAcceptResponse;

type tChangeRequest = (client: Client, f: tChangeFunction);
event eChangeRequest: tChangeRequest;
type tChangeResponse = (proposer: Proposer, value: tState);
event eChangeResponse: tChangeResponse;

type tStore = (value: tState);
event eStore: tStore;
