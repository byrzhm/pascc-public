program RecordRefParamExample;

type
    Student = record
        name: string;
        age: integer;
        grade: real;
    end;

var
    student1: Student;

procedure UpdateStudentGrade(var s: Student; newGrade: real);
begin
    s.grade := newGrade;
end;

begin
    student1.name := 'John Doe';
    student1.age := 20;
    student1.grade := 85.5;

    writeln('Old Grade: ', student1.grade);
    UpdateStudentGrade(student1, 90.5);
    writeln('New Grade: ', student1.grade);
end.