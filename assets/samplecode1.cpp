int ReturnZero(int num)
{
  if(num)
  {
    //test with a really long line
    int this_is_a_REAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAALLY_LOOOOOOOOOOOOOOOOOONG_NAAAAAAME = 9482390423098419 * 874230923 * 123908123 + 1293810923091 - 9182309490123 + 832942390902342.01249812901;
    return 0;
  }
  else
  {
    for(int i = 0; i < num; ++i)
    {
      if(i % 2 == 0)
        break;
      else
        continue;
    }
  }
  return 0;
}