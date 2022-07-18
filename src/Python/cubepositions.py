#!/usr/bin/env python
import numpy as np
from itertools import permutations

# split the master node so we get a unique cube config
# Fix definition of port direction now we use
# left, back, bottom
# with indices 0,8,9
# think of these as 0a 0b and 0c
# the rest is labelled 1-7

adjacencyindices =[
    [1,], # 0 top right front: 0a
    [0,2,5], # 1 top left front connects to 0a
    [1,3,6], # 2 top left back
    [8,2,7], # 3 top right back connects to 0b
    [9,5,7], # 4 bottom right front connects to 0c
    [1,4,6], # 5 bottom left front
    [2,5,7], # 6 bottom left back
    [3,4,6],  # 7 bottom right back
    [3,], # 8 top right front: 0b
    [4,], # 9 top right front: 0c
]


def random_cube_config():
    """
    Return a random permutation
    Always start with master cube 0 and add 8,9
    """
    c = np.random.permutation(7) + 1
    c = np.hstack([0, c, 8, 9])
    return c

def config_to_adjacency_matrix(c, adjacencyindices=adjacencyindices):
    """
    Construct an adjacency matrix based on a config.
    Translation is done based on the adjacencyindices table
    """

    A = np.zeros([10,10])
    for idx, (id, nids) in enumerate(zip(c, adjacencyindices)):
        row = id
        for n in nids:
            col = c[n]
            A[row, col] = 1
            A[col, row] = 1

    return A


def all_configurations():
    cfgs = []

    for p in permutations(np.arange(7)+1):
        cfg = np.hstack([0,p,8,9])
        cfgs.append(cfg)

    return cfgs


def adjacency_matrix_to_config(A):
    """
    Not unique!
    This will return 6 equivalent results.
    Brute force, because there are only 7! = 5040 possibilities
    """
    res = []
    for p in permutations(np.arange(7)+1):
        cfg = np.hstack([0,p,8,9])
        if np.all( config_to_adjacency_matrix(cfg) == A):
            res.append(cfg)
    return res

def score_configs(cfga, cfgb):
    return np.sum(cfga == cfgb)

def best_config(cfgs, cfg_ref):
    """
    Input a list of (equivalent) configurations
    Selects the first config that has the highest score of all
    configurations in cfgs.
    Not the optimal soltion.
    We should probably solve the uniqueness problem before we get here...
    """
    best_score = 0
    best_cfg = None
    for c in cfgs:
        s = score_configs(c, cfg_ref)
        if s > best_score:
            best_score = s
            best_cfg = c
    return best_cfg, best_score - 2 # substract the scores for 0b and 0c


def is_cube(A):
    """
    Verify the adjacency matrix is one that belongs to a cube
    """
    symmetric = np.all(A==A.T)
    all_connected = np.all(np.add.reduce(A) == 3)
    return symmetric & all_connected


if __name__ == "__main__":
    adj = adjacencyindices

    cfg = random_cube_config()
    cfgtest = random_cube_config()
    print("Correct cube config (randomly generated): %s"%(cfg))
    print("Test configuration: %s"%(cfgtest))
    A = config_to_adjacency_matrix(cfg)
    Atest = config_to_adjacency_matrix(cfgtest)

    #Aref = config_to_adjacency_matrix(range(10))
    #cfgFromAref = adjacency_matrix_to_config(Aref)

    cfgFromAtest = adjacency_matrix_to_config(Atest)
    cfgFromA = adjacency_matrix_to_config(A)

    print("Test configuration from adjacency matrix: %s"%(cfgFromAtest))
    print( "Score of test: %d"%(best_config(cfgFromAtest, cfg)[1] ))
    print( "Score of solution: %d"%( best_config(cfgFromA, cfg)[1] ))

    X = np.vstack([np.identity(10)[c][1:-2] for c in all_configurations()])
    Y = np.vstack([ config_to_adjacency_matrix(c)[1:-2] for c in all_configurations()])
