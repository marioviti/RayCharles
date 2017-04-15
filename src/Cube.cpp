  #include "Cube.h"

void Cube::buildMesh(int N) {
/*
      G----------------H
     /                /|
   /                /  |
  C--------------D/    |
  |               |    |
  |               |    |
  |    F          |    E
  |               |   /
  |               | /
  A--------------B/
*/

  V.resize(8);
  T.resize(12);

  // First face ABCD
  Vec3 A = Vec3(centre[0]-0.5*side,centre[1]-0.5*side,centre[2]+0.5*side);
  Vec3 B = Vec3(centre[0]+0.5*side,centre[1]-0.5*side,centre[2]+0.5*side);
  Vec3 C = Vec3(centre[0]-0.5*side,centre[1]+0.5*side,centre[2]+0.5*side);
  Vec3 D = Vec3(centre[0]+0.5*side,centre[1]+0.5*side,centre[2]+0.5*side);

  // First face trigs
  // clock wise order will make normal face away outside
  // ABD trig
  T[0].v[0] = 0;
  T[0].v[1] = 1;
  T[0].v[2] = 3;
  // DCA trig
  T[1].v[0] = 3;
  T[1].v[1] = 2;
  T[1].v[2] = 0;

  V[0].p = A;  V[1].p = B;  V[2].p = C;  V[3].p = D;

  // Second face BEDH
  Vec3 E = Vec3(centre[0]+0.5*side,centre[1]-0.5*side,centre[2]-0.5*side);
  Vec3 H = Vec3(centre[0]+0.5*side,centre[1]+0.5*side,centre[2]-0.5*side);

  //BEH trig
  T[2].v[0] = 1;
  T[2].v[1] = 4;
  T[2].v[2] = 5;

  //HDB trig
  T[3].v[0] = 5;
  T[3].v[1] = 3;
  T[3].v[2] = 1;

  V[4].p = E;  V[5].p = H;

  // Third face EFHG
  Vec3 F = Vec3(centre[0]-0.5*side,centre[1]-0.5*side,centre[2]-0.5*side);
  Vec3 G = Vec3(centre[0]-0.5*side,centre[1]+0.5*side,centre[2]-0.5*side);

  //EFG trig
  T[4].v[0] = 4;
  T[4].v[1] = 6;
  T[4].v[2] = 7;

  //GHE trig
  T[5].v[0] = 7;
  T[5].v[1] = 5;
  T[5].v[2] = 4;

  V[6].p = F;  V[7].p = G;

  // 4rth face FAGC

  //ACG trig
  T[6].v[0] = 0;
  T[6].v[1] = 2;
  T[6].v[2] = 7;

  //GFA trig
  T[7].v[0] = 7;
  T[7].v[1] = 6;
  T[7].v[2] = 0;

  // 5th face CDGH

  //CDH trig
  T[8].v[0] = 2;
  T[8].v[1] = 3;
  T[8].v[2] = 5;

  //HGC trig
  T[9].v[0] = 5;
  T[9].v[1] = 7;
  T[9].v[2] = 2;

  // 6th face AFBE

  //AFE trig
  T[10].v[0] = 0;
  T[10].v[1] = 6;
  T[10].v[2] = 4;

  //EBA trig
  T[11].v[0] = 4;
  T[11].v[1] = 1;
  T[11].v[2] = 0;

  recomputeNormals ();
  buildArray();
}
