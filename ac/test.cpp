namespace ac {
  class A {
  public:
    enum B {
      k1,
      k2
    };

  };
}
int main() {
  // ac::AttrVal val = 1;
  ac::A::B testval = ac::A::B::k1;

}
