import { Component, OnInit } from '@angular/core';
import {FileUploadService} from "../services/file-upload.service";
import {FileUpload} from "../model/file-upload.model";


@Component({
  selector: 'app-uploadcomp',
  templateUrl: './uploadcomp.component.html',
  styleUrls: ['./uploadcomp.component.css']
})
export class UploadcompComponent implements OnInit {
  selectedFiles?: FileList;
  currentFileUpload?: FileUpload;
  percentage = 0;
  xor = 42;
  times = 7;

  constructor(private uploadService: FileUploadService) { }

  ngOnInit(): void {
  }

  selectFile(event: any): void {
    this.selectedFiles = event.target.files;
  }

  upload(): void {
    if (this.selectedFiles) {
      const file: File | null = this.selectedFiles.item(0);
      this.selectedFiles = undefined;

      if (file) {
        this.currentFileUpload = new FileUpload(file, this.times.toString(), this.xor.toString());
        this.uploadService.pushFileToStorage(this.currentFileUpload).subscribe(
          percentage => {
            this.percentage = Math.round(percentage ? percentage : 0);
          },
          error => {
            console.log(error);
          }
        );
      }
    }

  }
}
