Reference: PAEA Loop [#f1]_
=====================================

Following `Diátaxis <https://diataxis.fr/reference/>`_, this section is designed
to be a reference guide.


Main script
-----------

There is a `main script <https://github.com/osu-particle-astrophysics/GENETIS_
HPol/tree/d/Evolutionary_ loop/Loop_Scripts>`_ called ``main.sh`` inside
``GENETIS_HPol/Evolutionary_loop/Loop_Scripts/``.

..  todo::
    
    should I be using ``GENETIS_HPol`` branch ``d`` or branch ``main`` in the 
    link above?

    make sure glossary is up-to-date

.. -----------------------------------------------------------------------------

general variables
^^^^^^^^^^^^^^^^^
.. glossary::

    RunName
        Should be unique like we all are.

    Design
        Antenna type: for example, ``"PUEO"``, or ``"HPol"``. 
        See :ref:`gentree`.

    TotalGens
        Number of generations; we start counting generations from ``0``.

    NPOP 
        Number of individuals (ie. antennas) per generation.

        Please keep below ``99``. We usually set this to ``50``.

    Seeds
        The number of AraSim jobs for each individual. 
        
        Suppose, for each of the :term:`NPOP` antennas in a generation, we
        would like to simulate its reponse to a **total** of 300-thousand
        neutrinos using the software AraSim.  Our way to achieve this number is
        to first note that, for example, :math:`15`\ k :math:`* 20 = 300`\ k;
        consequently, we let ``NNT=15000`` and ``Seeds=20``, see below.

    NNT 
        Number of Neutrinos Thrown in a single AraSim job.
        Increasing ``NNT`` reduces error but slows down AraSim as well.

        Following the example from :term:`Seeds`, we divide the 300k-neutrino
        AraSim simulation into 20 smaller jobs and run them in parallel to speed
        things up.

    exp
        AraSim neutrino energy exponent.
        Currently, we run at ``exp=18``.

    ScaleFactor
        Factor used to punish antennas that grow too large (larger than the
        Askaryan Radio Array boreholes)

    GeoFactor
        Factor used to scale *down* antennas
       

genetic algorithm (GA) variables
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
..  todo::

    * check with Ryan or Dylan to see if these are accurate
    * check if the units are correct
    * reference links to reproduction, cressover, etc.


..  glossary::

    REPRODUCTION
        number of individuals formed through reproduction

    CROSSOVER
        number of individuals formed through crossover
    
    MUTATION
        probablity of mutation (unit: percentage)

    SIGMA
        standard deviation for the mutation operator (unit: percentage)

    ROULETTE
        portion of individuals selected by roulette

    TOURNAMENT
        portion of individuals selected by tournament 

    RANK
        portion of individuals selected by tournament 

    ELITE
        ``elite`` function on/off switch (``1`` for on)


PUEO-specific variables
^^^^^^^^^^^^^^^^^^^^^^^
..  todo::

    define :term:`SYMMETRY` and :term:`XFCOUNT`?

..  glossary::

    SYMMETRY
        foo

    XFCOUNT
        bar


VPol-specific variables
^^^^^^^^^^^^^^^^^^^^^^^
..  todo::

    *   a bicone figure here?
    *   geometry-related variable definitions are probably wrong

..  glossary::

    RADIUS
    LENGTH
    ANGLE
        These define the geometry of a bicone antenna.

    CURVED
        On/off switch for curved bicone (0 for bicones with straight sides)

    A
    B
        These define the shape of the bicone-antenna sides. The curve of the
        "cones" is given by :math:`y = A \cdot x^2 + B`

    SEPARATION
        On/off switch for the evolution of the separation distance between the
        two cones of a bicone-antenna (defaults to 0: constant separation)

    NSECTIONS
        This defines the symmetry of the bicone (``1`` for symmetric bicones,
        ``2`` for assymetric ones).

        If any of the variables :term:`RADIUS`, :term:`LENGTH`, :term:`ANGLE`,
        :term:`A`, or :term:`B` is made asymmetric, :term:`NSECTIONS` needs
        to be changed accordingly.


flags
^^^^^
..  glossary::

    DEBUG_MODE
        On/off switch for debug mode (0 for real runs).


XF-specific variables
^^^^^^^^^^^^^^^^^^^^^
..  glossary::

    num_keys
        number of XF licenses
    
    FREQ
        number of entries inside the list of frequencies


Save-State
^^^^^^^^^^
..  todo::
    
    more explanation on ``indiv``?

Before we move on from the reference guide of ``main.sh`` into its different
parts, it is apt to introduce ``SaveState.txt`` file inside the
``Evolutionary_loop/SaveStates`` directory, a file containing just three lines
of integers. We'll use an example to illustrate the usage of this file.

..  glossary::
    
    InitialGen
        Generation. Suppose the loop was interrupted and the 28\ :sup:`th`
        generation did not finish running. When we start over, we would like to
        start with the 28\ :sup:`th` generation instead of wasting hours
        repeating the previous generations. In this case, then, ``InitialGen``
        would be ``28``, and we would pick up this piece of information from the
        ``SaveState.txt`` file.
    
    state
        Think of this as "checkpoints". ``main.sh`` has different :ref:`sections
        <loop_chart_simple>`: part (A) through (F). When each section finishes
        running, ``state`` is incremented and stored in ``SaveState.txt`` so
        that next time we run the 28\ :sup:`th` generation (following the
        example from :term:`InitialGen` above), we don't have to repeat the
        sections that are already completed.
    
    indiv
        individual

``SaveState.txt`` is created by
``GENETIS_HPol/Evolutionary_loop/SaveState_prototype.sh``

.. ----------------------------------------------------------------------------

Loop Parts
----------
``main.sh`` is divided into several sections, corresponding to the scripts found
inside the directory ``Evolutionary_loop/Loop_Parts/``.

..  toctree::
    :maxdepth: 1

    A3_1_1
    A3_1_2
    A3_1_3
    A3_1_4
    A3_1_5
    A3_1_6

..  rubric:: reference
..  [#f1] Rolla, Julie. Dissertation Appendix A. Section A.3.1