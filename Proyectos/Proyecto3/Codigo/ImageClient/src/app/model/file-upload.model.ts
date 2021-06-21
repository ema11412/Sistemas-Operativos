export class FileUpload {
  key!: string;
  name!: string;
  url!: string;
  file: File;
  times!: string;
  xorkey!: string;

  constructor(file: File, times: string, xorkey: string) {
    this.file = file;
    this.times = times;
    this.xorkey = xorkey;
  }
}
