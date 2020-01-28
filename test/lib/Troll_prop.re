open Framework;
open QCheckRely;
open Generator.Fantasy;
open Lib.Troll;
open Lib.Elf;

let {describe} = extendDescribe(QCheckRely.Matchers.matchers);

describe("Troll Invariance", ({test}) => {
  test("Troll score should be 0 when all elves resurrected", ({expect}) => {
    QCheck.Test.make(
      ~count=1000,
      ~name="Troll score should be 0 when all elves resurrected",
      troll_arbitrary,
      troll =>
      all_elves_resurrected(troll) |> scoring == 0
    )
    |> expect.ext.qCheckTest;
    ();
  });
  test("Troll score should always be >= 0", ({expect}) => {
    QCheck.Test.make(
        ~count=1000,
        ~name="Troll score should always be >= 0",
        troll_arbitrary,
        troll =>
        scoring(troll) >= 0
      )
      |> expect.ext.qCheckTest;
    ()
  });
});

describe("Troll Inverse", ({test}) => {
  test("oops_he_survived should always be inverse of i_got_one", ({expect}) => {
    QCheck.Test.make(
        ~count=1000,
        ~name="oops_he_survived should always be inverse of i_got_one",
        troll_elf_arbitrary,
        ((troll, elf)) =>
        scoring(troll |> i_got_one(elf) |> oops_he_survived(elf)) == scoring(troll)
      )
      |> expect.ext.qCheckTest;
    ()
  })
});



describe("Troll Analogy", ({test}) => {
  test("i_got_one and i_got should be consistent", ({expect}) => {
    QCheck.Test.make(
        ~count=1000,
        ~name="i_got_one and i_got should be consistent",
        troll_elf_int_arbitrary,
        ((troll, elf, x)) => {
        let tmptroll = ref(troll);
          for (y in 1 to x) {
            tmptroll := i_got_one(elf,tmptroll^);
          };
          scoring(tmptroll^) == scoring(i_got(x, elf, troll))
        }
      )
      |> expect.ext.qCheckTest;
    ()
  })
});

describe("Troll Idempotence", ({test}) => {
  test(
    "all_elves_of_a_kind_resurrected brings the Troll killing list to a stable state",
    ({expect}) => {
      QCheck.Test.make(
        ~count=1000,
        ~name="all_elves_of_a_kind_resurrected brings the Troll killing list to a stable state",
        troll_elf_int_arbitrary,
        ((troll, elf, x)) => {
          let tmptroll = ref(troll);
            for (y in 1 to x) {
              tmptroll := all_elves_of_a_kind_resurrected(elf,tmptroll^);
            };
            x > 0 ? 
            scoring(tmptroll^) == scoring(all_elves_of_a_kind_resurrected(elf,troll))  : true;
          }
        )
        |> expect.ext.qCheckTest;
      ()
  })
});

describe("Troll Metamorphisme", ({test}) => {
  test(
    "i_got_one increases score when an elf is killed",
    ({expect}) => {
      QCheck.Test.make(
        ~count=1000,
        ~name="i_got_one increases score when an elf is killed",
        troll_elf_arbitrary,
        ((troll, elf)) => {
          troll |> i_got_one(elf) |> scoring == scoring(troll) + value(elf)
        }
        )
        |> expect.ext.qCheckTest;
      ()
  })
});

describe("Troll Injection", ({test}) => {
  test(
    "the score after i_got_one is different after the troll kills 2 different elfes",
    ({expect}) => {
      QCheck.Test.make(
        ~count=1000,
        ~name="the score after i_got_one is different after the troll kills 2 different elfes",
        troll_two_elves_arbitrary,
        ((troll, elf1, elf2)) => {
          let troll1 = i_got_one(elf1, troll);
          let troll2 = i_got_one(elf2, troll1);
          scoring(troll1) < scoring(troll2);
        }
      ) |> expect.ext.qCheckTest;
      ();
  })
});