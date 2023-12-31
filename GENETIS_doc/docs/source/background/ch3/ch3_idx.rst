Antenna Optimization with Genetic Algorithms [#f1]_
=====================================================

Navigation
----------
This chapter begins with an :ref:`introduction to the
GENETIS project <ch3_1>`, then describes the :doc:`early projects <ch3_2>` and
the :doc:`current GENETIS algorithm <ch3_3/ch3_3_idx>`. 
Finally, we discuss the **two main** applications of the GA: 
:doc:`evolving physical antennas <ch3_4/ch3_4_idx>` and 
:doc:`evolving gain patterns <ch3_5/ch3_5_idx>`.

..  todo::

    *   include other GENETIS projects
    *   **gain pattern** glossary here

..  ----------------------------------------------------------------------------
..  _ch3_1:

Why GENETIS
-----------

Overview
^^^^^^^^
With much of its work being undergraduate-driven, the Genetically Evolving
NEuTrIno TeleScopes (GENETIS) project started in 2017 at The Ohio State
University. 
The goal of the project is to **optimize the science outcome of detector
designs** in high-dimensional parameter spaces to advance the field of
astroparticle physics.
The project is rare in its application of machine learning for designing an
antenna using a science outcome as the sole measure of fitness in a genetic
algorithm (GA).

Inspiration
^^^^^^^^^^^
GAs have previously been applied to the design of various detectors and
experiments although seldom used to optimize for a science outcome directly.
Some examples of evolution toward a science outcome include a horn antenna
designed using a GA optimized for detecting Cosmic Microwave Background
radiation. Another example is from both the Long Baseline Neutrino Oscillation
experiment (LBNO) and the Deep Underground Neutrino Experiment (DUNE), where GAs
were utilized to optimize the design of neutrino beamlines using simulations of
a science outcome to determine the fitness. GAs have also been used to optimize
the layout of detectors, sensors, shielding, and for trigger optimization.

Our goal
^^^^^^^^^^^^
GENETIS seeks to assist the search for one of the most important missing piece
of particle astrophysics -- the detection of ultra-high energy (UHE) neutrinos
at energies above ~\ :math:`10^{18}`\ eV. 
Utilizing a variety of different antennas, a number of experiments employ
antenna arrays to detect Askaryan radiation produced from a neutrino-ice
interaction in Antarctica or Greenland:

1.  ANtarctic Impulsive Transient Antenna (ANITA), now named PUEO
2.  Askaryan Radio Array (ARA)
3.  Antarctic Ross Ice-Shelf Antenna Neutrino Array (ARIANNA)
4.  Radio Neutrino Observatory in Greenland (RNO-G)

The initial GENETIS goal is to explore optimizing some of the
current detector designs of these major experiments via the use of GAs.
As a first application, GENETIS produced a GA that evolves ARA antennas.


Why Genetic Algorithms
----------------------
Problem at hand
^^^^^^^^^^^^^^^
The high-dimensional parameter spaces of detector design problems motivate
computational methods to improve upon designs made using traditional techniques.
In particular, the design of antennas for UHE neutrino detection has explicit
constraints and a large parameter space, which makes it well suited for machine
learning. Given the immense scale of these experiments and the difficulty in
detecting UHE neutrinos, each detector element must be designed to return the
most science for its cost. 

Inspiration
^^^^^^^^^^^^
The use of GAs was initially motivated by the NASA ST-5 antenna in which a GA
designed a simple, segmented, wire antenna for satellite communications. 
Many other examples exist of antenna design optimization using GAs including
Yagi-Uda antennas, electrically loaded wire antennas, broadband cage antennas,
planar antennas, pyramid horn antennas, ultra-wideband slot antennas, helical
antennas, patch antennas, adaptive antennas and others . 

GAs were chosen because of their effectiveness at complex optimization problems,
especially when many optima could exist.
For instance, searching a 6-dimensional parameter space (as is the case for the
asymmetric bicone discussed in Chapter **3.4.2**), using increments necessary to
find a peak fitness score would require evaluation of more than :math:`10^{8}`
designs.
In comparison, the results presented required only 1550 antenna designs to
search the same parameter space with the GA.

..  todo:: 

    reference 3.4.2

GAs are also more transparent than other optimization techniques, which allows
for a better understanding of how the algorithm arrived at a final result
instead of a black-box model.

.. The early endeavors of GENETIS mostly involved proof of concept designs and
.. tests that are covered in Chapter~\ref{ch:early}. The first project in 2017 used
.. a GA to evolve to the known length of a quarter wavelength dipole antenna at 3
.. GHz. Other early work involved the evolution of a paperclip antenna toward set
.. patterns and performance tests.  

.. Chapter~\ref{ch:GENETISGA} describes the heart of the GENETIS project, the
.. custom genetic algorithm. Each step of the GA is discussed in detail, covering
.. the initialization, fitness evaluation, new generation creation, and
.. termination. What makes the GENETIS GA impressive is the integration of various
.. types of simulation software to generate a fitness score. This chapter and
.. Appendix~\ref{genmanual} discuss these programs in detail.  

.. The second stage of investigation is the Physical Antenna Evolution Algorithm (PAEA) project and is discussed in Chapter~\ref{ch:PAEA}. PAEA uses the GENETIS GA to investigate the optimization of the ARA collaboration's in-ice vertical polarization bicone antennas. PAEA initially optimized  a symmetric bicone antenna with linear sides, and is now exploring more complex geometry, such as (1) asymmetric bicone with linear sides and (2) asymmetric bicone with nonlinear sides. The results of each project are discussed below. 

.. As a third investigation, GENETIS is optimizing the antenna response pattern, without any antenna designs or geometry. This investigation, called the Antenna Response Evolutionary Algorithm (AREA), is being run with minimal constraints, as the goal of this project is to explore what improvement to the neutrino sensitivity is possible due to improvements in antenna responses alone, without regard to what physical design might be needed to bring about that response. The results of this investigation are presented in Chapter~\ref{ch:AREA}. 

.. % Future goals for GENETIS include optimization of detectors for similar experiments, such as the ANITA horn antennas, and exploring the optimization of a plethora of other experimental parameters, such as array design and detector layout. 

.. My contributions to GENETIS involve heavy involvement in creating the loop and, more recently, as a mentor and leader for the group. Over time, the early code has grown, and our network of primary contributors, which now includes myself, 11 undergraduate students, and experts in the fields of GAs, ML, antennas, and additive manufacturing. As one of the significant architects of our current software package, I have been guiding all GENETIS projects and working alongside undergraduate students in the drive to make meaningful contributions. I wrote the user guide to the loop, which is necessary to understand all of the complex moving parts. Additionally, I built a training course that on-boards students and prepares them to contribute. This course includes information on the big picture of UHE neutrino experiments, GAs, and the GENETIS group, and instructs them in the principles of coding in the different languages necessary to improve the GA. Furthermore, I led weekly planning and working meetings and developed task tracking and prioritization sheets to help students stay on track. 


Discussion: GENETIS Projects
----------------------------

..  toctree::

    ch3_2
    ch3_3/ch3_3_idx
    ch3_4/ch3_4_idx
    ch3_5/ch3_5_idx
    ch3_6

.. -----------------------------------------------------------------------------

..  rubric:: reference
..  [#f1] Rolla, Julie. Dissertation Chapter 3.1