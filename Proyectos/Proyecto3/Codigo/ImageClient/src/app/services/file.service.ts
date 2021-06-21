import { Injectable } from '@angular/core';
import { HttpClient, HttpHeaders } from '@angular/common/http';
import { Observable } from 'rxjs';
import { Image } from '../model/Image';
import { Result } from '../model/Result';

const httpOptions = {
  headers: new HttpHeaders({
    'Content-Type': 'application/json'
  })
};

@Injectable({
  providedIn: 'root'
})
export class FileService {
  apiUrl = 'https://file.io';
  constructor(private http: HttpClient) {
  }
  upload(file): Observable<any> {
    const formData = new FormData();
    console.log(file);
    formData.append('file', file, file.name);
    return this.http.post(this.apiUrl, formData);
  }
  sendImage(img: Image[], url: string): Observable<Result>{
     return this.http.post<Result>(`${url}contraste`, img, httpOptions);
  }
}
