type NomeAluno = String
type MediaNota = Int
type Aluno = (NomeAluno, MediaNota) 
type Turma = [Aluno]

aprovados turma nota = [nome | (nome,media) <- turma, media >= nota]

type Ponto = (Int, Int, Int)

distancia (x1,y1,z1) (x2, y2, z2)= sqrt(dx^2 + dy^2 + dz^2)
    where
        dx = x1-x2
        dy = y1-y2
        dz = z1-z2