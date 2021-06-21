import { Component, OnInit } from '@angular/core';
import { SwUpdate } from '@angular/service-worker';
import { FileService } from './services/file.service';
import { Result } from './model/Result';
import { Image } from './model/Image';
import { FormControl, FormGroup, Validators, FormBuilder } from "@angular/forms";
import { ThemePalette } from "@angular/material/core";

@Component({
  selector: 'app-root',
  templateUrl: './app.component.html',
  styleUrls: ['./app.component.css']
})
export class AppComponent implements OnInit {
  title = 'ImageClient';
  files: Array<any>;
  user = new FormControl('', [Validators.required]);
  sel_function = new FormControl('', [Validators.required]);
  fileControl: FormControl;
  options: FormGroup;
  color: ThemePalette = 'primary';
  multiple: boolean = false;
  xor: string = "42";
  times: string = "7";
  id_function: string;
  accept: string;
  ip: string;
  port: string;
  sent: boolean = false;
  update = false;
  loading = false;
  file: File = null;
  fileService: FileService;
  images: Image[];
  response: Result;
  constructor(updates: SwUpdate, fileService: FileService, fb: FormBuilder) {
    this.xor = '42';
    this.times = '7';
    this.images = [];
    this.ip = '0.0.0.0';
    this.port = '1717';
    this.accept = ".png,.jpg,.jpeg";
    this.fileService = fileService;
    this.options = fb.group({
      bottom: 0,
      fixed: false,
      top: 0
    });
    this.fileControl = new FormControl(this.files, [Validators.required]);
    updates.available.subscribe(event => {
        // this.update = true;
        updates.activateUpdate().then(() => document.location.reload());
    });
  }
  ngOnInit() {
    this.fileControl.valueChanges.subscribe((file: File) => {
      this.file = file;
      this.uploadFile();
    })
  }
  getErrorUsername(): string {
    if (this.user.hasError('required')) {
      return 'Debes ingresar un nombre de usuario';
    }
  }
  getIdFunction(text: string): string {
    var val;
    switch (text) {
      case "Histograma de ecualización": {
        val = "1"
        break;
      }
      case "Color predominante": {
        val = "2"
        break;
      }
      default: {
        val = "0"
        break;
      }
    }
    return val
  }
  uploadFile(): void {
    this.loading = !this.loading;
    this.sent = false;
    console.log(this.id_function);
    this.fileService.upload(this.file).subscribe((event: any) => {
        if (typeof (event) === 'object') {
          this.images.push({
            name: this.file.name,
            link: event.link,
            xor: this.xor,
            times: this.times
          });
          this.loading = false;
        }
      }
    );
  }
  sendImage(): void {
    if (!this.user.invalid) {
      console.log(this.images[0].name);
      console.log(this.images[0].link);
      this.user.setErrors({'invalid': false});
      console.log(`http://${this.ip}:${this.port}/`);
      this.fileService.sendImage(this.images, `http://${this.ip}:${this.port}/`).subscribe(event => {
        this.response = event;
        this.images = [];
        this.sent = true;
        console.log(this.response.message);
      });
    }
  }
}
