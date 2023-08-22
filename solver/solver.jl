using JuMP
using GLPK
using Formatting

open("../cmb/cmb01") do file
  ln = readline(file)
  K_Total = parse(Int64, split(ln, ' ')[3])
  N_Total = parse(Int64, split(ln, ' ')[1])

  ln = readline(file)
  l = split(ln, ' ')
  W = Array{Float64}(undef, N_Total)
  E = Array{Int64}(undef, N_Total, N_Total)
  for i = 1:N_Total
    W[i] = parse(Float64, l[i])
    for j = 1:N_Total
      E[i,j] = 0
    end
  end

  while !eof(file)
    ln = readline(file)
    i = parse(Int64, split(ln, ' ')[1]) + 1
    j = parse(Int64, split(ln, ' ')[2]) + 1
    E[i, j] = 1
    E[j, i] = 1
  end

  K = collect(1:K_Total); N=collect(1:N_Total);

  m = Model()
  set_optimizer(m, GLPK.Optimizer);
  @variable(m, C[k in K, i in N], Bin);
  @objective(m, Min, sum(C[1, i] * W[i] for i in N))

  @constraint(m, [[i,j] in (E[i,j] == 1), k in K], C[k,i] + C[k,j] <= 1);

  @constraint(m, [i in N], sum(C[k,i] for k in K) == 1);

  @constraint(m, [k in K], sum(C[1,i] * W[i] for i in N) .>= sum(C[k,i] * W[i] for i in N));

  optimize!(m)

  println(objective_value(m))
end