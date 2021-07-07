# N-Body-Simulation

## Demos:

### Initial Spawning of Stars:

N = 750 bodies; 
the initial spawnpoint for each star is a randomized position-vector p within a 3-dimensional sphere around origin such that each coordinate of position p is in the range [0,1]:
initial_p = {p | p.x, p.y, p.z is in range [0,1]}
This compact spawnpoint structure allows us to immediately see the strong gravitational attractions between those n arbitrary stars. For example, the 'slingshot effect' prominently takes place when stars orbit in close proximity.

https://user-images.githubusercontent.com/31304414/124673639-19781100-de6e-11eb-8f53-5d78e18064c1.mp4

It must be noted that some manipulations were made when calculating the force vector between 2 arbitrary stars. Namely, the cubic term of a softening factor: e_soft^3=0.005^3 was added to the denominator when calculating the gravitational forces between 2 bodies such that:

f_ij = (G * m_1 * m_2 * r_ij) / (r_ij_norm^3 + e_soft^3); 
where f_ij is the force-vector between 2 bodies i and j and r_ij is the displacement vector between bodies i and j.

This was necessary, as our simulation does not account for star collision, and therefore without this softening factor, an unbounded growth in the gravitational calculations was produced. The detrimental implications of this were 'slingshot effects' inconsistent with reality.

### Initial Spawning of Stars (1.5x speed):

https://user-images.githubusercontent.com/31304414/124674793-4299a100-de70-11eb-88ef-da55c36e62e4.mp4

### Initial Spawning of Stars (2.0x speed):

https://user-images.githubusercontent.com/31304414/124674994-a15f1a80-de70-11eb-8b2b-05b62fae7961.mp4

### Camera Movement Around Star Cluster:

https://user-images.githubusercontent.com/31304414/124675608-d029c080-de71-11eb-8129-81f459421e28.mp4

### Star Geometry

![star geometry 1](https://user-images.githubusercontent.com/31304414/124677125-a45c0a00-de74-11eb-8f9e-c6f6776b937b.png)

![star geometry 2](https://user-images.githubusercontent.com/31304414/124677135-a6be6400-de74-11eb-9dda-269b6f743202.png)

![star geometry 3](https://user-images.githubusercontent.com/31304414/124677116-a02fec80-de74-11eb-9393-a1aa715f6d8f.png)

### Demo of Small N for Better Performance

https://user-images.githubusercontent.com/31304414/124677095-973f1b00-de74-11eb-80ca-ab8b798de520.mp4


