type tBallot = int;
type tValue = int;

type tProposerConfig = (broadcast_kind: tBroadcast, jury: set[Acceptor], school: set[Learner], value_to_propose: tValue, id_proposer: int);
type tAcceptorConfig = (broadcast_kind: tBroadcast);
type tLearnerConfig = (broadcast_kind: tBroadcast);

type tPrepareRequest = (proposer: Proposer, number_ballot: tBallot, value: tValue);
event ePrepareRequest: tPrepareRequest;

type tPrepareResponse = (acceptor: Acceptor, promised: tBallot, value_accepted: tValue, number_accepted: tBallot);
event ePrepareResponse: tPrepareResponse;

type tAcceptRequest = (proposer: Proposer, number_ballot: tBallot, value: tValue);
event eAcceptRequest: tAcceptRequest;

type tAcceptResponse = (acceptor: Acceptor, accepted: tBallot);
event eAcceptResponse: tAcceptResponse;

type tLearn = (ballot: tBallot, value: tValue);
event eLearn: tLearn;
