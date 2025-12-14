enum tBroadcast {Unreliable, UnreliableMulti, Reliable, ReliableMajority}

fun Broadcast(broadcast: tBroadcast, machines: set[machine], e: event, payload: any) {
    assert sizeof(machines) > 0;
    if (broadcast == Unreliable) {
        BroadcastUnreliable(machines, e, payload);
        return;
    }
    if (broadcast == UnreliableMulti) {
        BroadcastUnreliableMulti(machines, e, payload);
        return;
    }
    if (broadcast == Reliable) {
        BroadcastReliable(machines, e, payload);
        return;
    }
    if (broadcast == ReliableMajority) {
        BroadcastReliableMajority(machines, e, payload);
        return;
    }
    assert false;
}

// Unreliably send event to every target, either zero or one times.
fun BroadcastUnreliable(machines: set[machine], e: event, payload: any) {
    var i: int;
    while (i < sizeof(machines)) {
        if (choose()) {
            send machines[i], e, payload;
        }
        i = i + 1;
    }
}


// Unreliably send event to every target, between zero and 3 times.
fun BroadcastUnreliableMulti(machines: set[machine], e: event, payload: any) {
    var i: int;
    var n: int;
    var k: int;

    while (i < sizeof(machines)) {
        k = choose(3);
        n = 0;
        while (n < k) {
            send machines[i], e, payload;
            n = n + 1;
        }
        i = i + 1;
    }
}

fun BroadcastReliable(machines: set[machine], e: event, payload: any) {
    var i: int;
    i = 0;
    while (i < sizeof(machines)) {
        send machines[i], e, payload;
        i = i + 1;
    }
}

// Send event reliably to a majority of targets, and unreliably to the remaining, between zero and 3 times.
fun BroadcastReliableMajority(machines: set[machine], e: event, payload: any) {
    var i: int;
	var n: int;
	var k: int;
	var majority: int;

	majority = sizeof(machines) / 2 + 1;

	while (i < sizeof(machines)) {
		k = 1;
		if (i >= majority) {
			k = choose(3);
		}
		n = 0;
		while (n < k) {
			send machines[i], e, payload;
			n = n + 1;
		}
		i = i + 1;
	}
}
