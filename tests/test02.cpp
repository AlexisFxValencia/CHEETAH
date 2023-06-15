#include <gtest/gtest.h>
#include "../src/Vector3.h"


TEST(VectorTest, VectorCreation) {
  Vector3 myvector = Vector3(1, 2, 3); 
  EXPECT_EQ(myvector.x, 1);
  EXPECT_EQ(myvector.y, 2);
  EXPECT_EQ(myvector.z, 3);
}

TEST(VectorTest, VectorMagnitude) {
  Vector3 vector_a = Vector3(1, 2, 3); 
  Vector3 vector_b = Vector3(2, 3, -4); 
  Vector3 vector_c = vector_a + vector_b;
  EXPECT_EQ(vector_c.x, 3);
  EXPECT_EQ(vector_c.y, 5);
  EXPECT_EQ(vector_c.z, -1);
}

