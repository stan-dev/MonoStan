functions {
  real foo0() {
    return 0.0;
  }
  real foo1(real x) {
    return 1.0;
  }
  real foo2(real x, real y) {
    return 2.0;
  }
}
parameters {
  real y;
}
model {
  y ~ normal(0,1);
}
