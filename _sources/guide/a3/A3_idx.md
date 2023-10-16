# PAEA Introduction [^f1]

```{eval-rst}
..  todo::

    check the following listing with Julie or Alex
```

(gentree)=

## GENETIS Tree

```{eval-rst}
..  todo::

    NEBULUS full name?
```

The OSU GENETIS team has several projects. These include the Physical Antenna
Evolution Algorithm (PAEA), Antenna Response Evolution Algorithm (AREA), and
**NEBULOUS**. Here we will focus on PAEA, which has three subprojects:

1. `GENETIS_PUEO`:

   Payload for Ultrahigh Energy Observation (PUEO) dual-polarization
   quad-ridged horn antenna evolution

2. `GENETIS_ARA`:

   Askaryan Radio Array (ARA) vertically polarized (VPol) bicone antenna
   evolution. The terms "VPol" and "bicone" are used synonymously.

3. `GENETIS_HPol`:

   Askaryan Radio Array (ARA) horizontally polarized (HPol) quad-slot
   cylindrical antenna evolution

:::{note}
The long-term plan is to combine all of these into one project with a
{term}`Design` switch to allow users to switch between the three.
:::

% -----------------------------------------------------------------------------

(a3idx)=

## The "Loop"

Before we begin, note that every GENETIS project has a
{doc}`main script <A3_1/A3_1_main>` that is essentially a `while` loop.
Consequently, a GENETIS project is simply referred to by the team as *the loop*.

:::{glossary}
Loop /'lüp/

> A colloquial term that refers to any GENETIS projects.
>
> "Ah! The `HPol` loop is broken again. I wonder who wrote this piece of
> garbage"
:::

```{eval-rst}
..  todo::

    *   figure needs to be updated
    *   figure needs a title

```

(loop-chart-simple)=

```{image} img/loop_chart.png
:scale: 60
```

% -----------------------------------------------------------------------------

We now begin introducing the PAEA project using `GENETIS_HPol` as an example.

```{toctree}
:caption: PAEA Reference Guide & How to Run the Loop

A3_1/A3_1_main
A3_2
```

```{rubric} reference
```

[^f1]: Rolla, Julie. Dissertation Appendix A. Section A.3
