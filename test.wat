(module
  (import "env" "printI" (func $pI(param i32)))
  ;;@ src.cpp:1:0
  (func $foo (param $a i32) (param $b i32)
    ;;@ src.cpp:1:1
    (block $b1
      ;;@ src.cpp:2:1
      (if
        ;;@ src.cpp:3:1
        (i32.le_s
          ;;@ src.cpp:4:1
          (local.get $a)
          ;;@ src.cpp:5:1
          (i32.const 1)
        )
        (block 
          ;;@ src.cpp:6:1
          (return)
          ;;@ src.cpp:7:1
          (local.set $a
            ;;@ src.cpp:8:1
            (i32.const 1)
          )
        )
        (local.set $a
          ;;@ src.cpp:9:1
          (i32.const 42)
        )
      )
    )
    ;;@ src.cpp:10:1
    (drop
      ;;@ src.cpp:11:1
      (i32.const 1)
    )
  )
)
