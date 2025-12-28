# CASPaxos

Compare-and-swap Paxos (CASPaxos) is a wait-free, linearizable, multi-writer multi-reader register in unreliable, asynchronous networks supporting arbitrary update operations including compare-and-set (CAS). The register acts as a replicated state machine providing an interface for changing its value by applying an arbitrary user-provided function (a command). [1]

## Model

Todo:

- Linearizable spec
- Test cases with unreliable broadcasts
- Arbitrary change function and state
- Cluster membership

## References

[1] https://github.com/rystsov/caspaxos/blob/master/latex/caspaxos.pdf
