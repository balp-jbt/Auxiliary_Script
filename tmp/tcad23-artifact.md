---
layout: default
title: Leveraging Datapath Propagation in IC3 for Hardware Model Checking
permalink: /research/tcad23-artifact
---

## <center>Leveraging Datapath Propagation in IC3 for Hardware Model Checking</center>

#### <center>Hongyu Fan and <a href="https://feihe.github.io/">Fei He</a></center>

## <center>Abstract</center>

IC3 is a famous bit-level framework for safety verification. By incorporating datapath abstraction, a notable enhancement in the efficiency of hardware verification can be achieved. However, datapath abstraction entails a coarse level of abstraction where all datapath operations are approximated as uninterpreted functions (UFs). This level of abstraction, albeit useful, can lead to an increased computational burden during the verification process as it necessitates extensive exploration of redundant abstract state space. In this article, we introduce a novel approach called datapath propagation. Our method involves leveraging concrete constant values to iteratively compute the outcomes of relevant datapath operations and their associated UFs. Meanwhile, we generate potentially useful datapath propagation lemmas in abstract state space and tighten the datapath abstraction. With this technique, the abstract state space can be reduced, and the verification efficiency is significantly improved. We implemented the proposed approach and conducted extensive experiments. The results show promising improvements of our approach compared to the state-of-the-art verifiers.

## <center>Datapath propagation

* Data-path propagation (DPP) is the core innovation of the paper.
* DPP Moderately repay the semantics of datapath operations according to the pre-established rules.
* During verification process, executes DPP prior to each invocation of the SMT solver.
* Lemmas can be generated during DPP, which are reused to optimize SMT solving.


## <center>Artifact</center>

The <a href="https://zenodo.org/records/10202559">artifact</a> contains all scripts, log files, benchmarks, and executable files in our evaluation. It is based on <a href="https://github.com/aman-goel/avr">avr</a> tool. Use `--wd` additionally to run datapath propagation approach in verifiation process. 

--------------------------------------

### Run artifact in small btor2 benchmarks

> Finish in a few minutes

* Run original AVR
  ```
  python3 run_exp_file.py --dir examples --mode check --wd
  python3 run_exp_file.py --dir examples --mode check
  ```

* Generate results excel file
  ```
  python3 run_exp_file.py --dir examples --mode generate --wd
  python3 run_exp_file.py --dir examples --mode generate
  ```
  
--------------------------------------

### Run artifact in full benchmarks

> May take dozens of hours

This section shows how to set up the artifact in all the benchmarks.

* Run original AVR
  ```
  python3 run_exp_file.py --mode check --wd
  python3 run_exp_file.py --mode check 
  ```

* Generate results excel file
  ```
  python3 run_exp_file.py --mode generate --wd
  python3 run_exp_file.py --mode generate 
  ```

--------------------------------------

### Directory Structure of the Artifact

The artifact consists of the following four directories:

  - `./build`

    Contains binary executable files built from source codes.
    

  + `./hwmcc19-20`

    Contains all the benchmarks used in evaluation. They are from the HWMCC'2019 and HWMCC'2020.

  + `./examples`

    Contains a few small benchmarks selected from `./hwmcc19-20`.
    

  + `./run_exp_file.py`

    The top script to run experiment.

  + `./*.py`

    The scripts to run experiment by executing binary files in `./build`.

  + `./deps`

    Contains the prerequisite files.
    

------------------------------------------------------------------------------
