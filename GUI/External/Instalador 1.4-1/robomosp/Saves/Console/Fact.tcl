proc Fact { n } {
  if { $n == 1 } {
    return 1
  }
  return [ expr $n * [ Fact [ expr $n - 1 ] ] ]
}

ConsoleWindow::Puts "Fact 5 = [ Fact 5 ]\n"