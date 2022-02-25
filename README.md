
#### Array Programming DSL

  
```
examples
idx ID3x3 = Cast(
	Space(3, 3, I32(0)),
	Graph(
		I32(1), I32(0), I32(0),
		I32(0), I32(1), I32(0),
		I32(0), I32(0), I32(1)
	)
);

idx ID3x3_scaled_by_25 = Mul(
	ID3x3,
	I32(25)
);
  

```