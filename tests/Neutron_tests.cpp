#include <gtest/gtest.h>
#include "../src/Neutron.h"


TEST(NeutronTest, set_random_position) {
  Neutron neutron1 = Neutron();
  neutron1.set_random_position();
  cout << "Created neutron's position is :" << neutron1.position << endl;
  cout << "Created neutron's max boundaries is :" << neutron1.max_boundaries << endl;
  cout << "Created neutron's min boundaries is :" << neutron1.min_boundaries << endl;
  EXPECT_LE(neutron1.position.x, neutron1.max_boundaries.x);
  EXPECT_GE(neutron1.position.x, neutron1.min_boundaries.x);
  EXPECT_LE(neutron1.position.y, neutron1.max_boundaries.y);
  EXPECT_GE(neutron1.position.y, neutron1.min_boundaries.y);
  EXPECT_LE(neutron1.position.z, neutron1.max_boundaries.z);
  EXPECT_GE(neutron1.position.z, neutron1.min_boundaries.z);
}

