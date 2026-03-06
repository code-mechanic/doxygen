# Lesson 7

This lesson focuses on documenting embedded state machines and recovery rules.

- state enums that mean something operationally
- legal and illegal API transitions
- timeout-driven state changes
- latched faults and explicit recovery entry

The sample is a simple power-sequencer driver. The goal is to make the docs tell
another engineer how the state machine behaves without reading the C file first.
