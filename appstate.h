#ifndef APPSTATE_H
#define APPSTATE_H

#include "datastructures.h"

// ─────────────────────────────────────────────────────────────
// AppState — single source of truth shared by every tab widget
// Each tab receives a pointer to this; when one tab mutates
// the data, other tabs can refresh from the same object.
// ─────────────────────────────────────────────────────────────
struct AppState {
    CandidateBST   candidateBST;
    VoterLinkedList voterList;
    VotingQueue    votingQueue;

    int nextCandidateId = 1;
    int nextVoterId     = 101;

    // Seed demo data so the app is not empty on first launch
    void seedDemo() {
        candidateBST.insert(nextCandidateId++, "KP Sharma Oli", "UML");
        candidateBST.insert(nextCandidateId++, "Balendra Shah",     "RSP");

        voterList.insert(nextVoterId++, "Bishwo Sharma");
        voterList.insert(nextVoterId++, "Narayan Wagle");

    }
};

#endif // APPSTATE_H
