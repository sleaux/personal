spec ChangeSucceeded observes eChangeRequest, eChangeResponse {

    var requests: map[Client, int];

    start state Init {
        entry {
            requests = default(map[Client, int]);
        }

        on eChangeRequest do (payload: tChangeRequest) {
            requests[payload.client] = payload.value;
        }

        on eChangeResponse do (payload: tChangeResponse) {
            assert requests[payload.client] == payload.value;
        }
    }
}
